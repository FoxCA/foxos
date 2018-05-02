#include <kernel.h>

char * current_directory;
list_t * dirstack;


int path_exists(char * name){
    if(!file_open(name, 0)) {
    	return 0;
    }
    return 1;
}

char * cd(char * directory){
	char * path = canonicalize_path(current_directory,directory); 
	if(path_exists(path)){
		current_directory = path;
	}else{
		printf("path not found\n");
	}
}

char * getcd(){
	return current_directory;
}

list_t * ls(char * directory){
   	list_t * ret = list_create();
   	char * path = canonicalize_path(current_directory,directory);
	vfs_node_t * n = file_open(path, 0);
	free(path);

    // Open the VFS node, call its list dir function
    if(!n) {
        printf("Could not list a directory that does not exist\n");
        return ret;
    }
    if(!n->listdir) return ret;
    char ** files = n->listdir(n);
    char ** save = files;
    while(*files) {
        list_insert_back(ret,strdup(*files));
        kfree(*files);
        files++;
    }
    kfree(save);
    return ret;
}

char * cat(char * filename){
	char * path = canonicalize_path(current_directory,filename);
    vfs_node_t * f = file_open(path, 0);
    free(path);
    if(!f) {
        printf("File does not exist\n");
        char * ret = malloc(1);
        ret[0] = '\0';
    	return ret;	
    }
    uint32_t size = vfs_get_file_size(f);
    // First, kmalloc a memory chunk for the file
    void * file = kmalloc(size);
    // Second, read from disk
	vfs_read(f, 0, size, file);
	return (char *)file;
}

void pushd(char * filename){
	char * directory = canonicalize_path(current_directory,filename);
	if(path_exists(directory)){
		list_push(dirstack,directory);
	}else{
		printf("path not found\n");
	}
}

void popd(){
	if(list_size(dirstack) != 0){
		current_directory = list_pop(dirstack);
	}else{
		printf("directory stack empty\n");
	}
}

void mkdir(char * directory){

}

void fs_init(){
	current_directory = "/";
	dirstack = list_create();
}

char *canonicalize_path(char *cwd, char *input) {
	/* This is a stack-based canonicalizer; we use a list as a stack */
	list_t *out = list_create();

	/*
	 * If we have a relative path, we need to canonicalize
	 * the working directory and insert it into the stack.
	 */
	if (strlen(input) && input[0] != PATH_SEPARATOR) {
		/* Make a copy of the working directory */
		char *path = malloc((strlen(cwd) + 1) * sizeof(char));
		memcpy(path, cwd, strlen(cwd) + 1);

		/* Setup tokenizer */
		char *pch;
		char *save;
		pch = strtok_r(path,PATH_SEPARATOR_STRING,&save);

		/* Start tokenizing */
		while (pch != NULL) {
			/* Make copies of the path elements */
			char *s = malloc(sizeof(char) * (strlen(pch) + 1));
			memcpy(s, pch, strlen(pch) + 1);
			/* And push them */
			list_push(out, s);
			pch = strtok_r(NULL,PATH_SEPARATOR_STRING,&save);
		}
		free(path);
	}

	/* Similarly, we need to push the elements from the new path */
	char *path = malloc((strlen(input) + 1) * sizeof(char));
	memcpy(path, input, strlen(input) + 1);

	/* Initialize the tokenizer... */
	char *pch;
	char *save;
	pch = strtok_r(path,PATH_SEPARATOR_STRING,&save);

	/*
	 * Tokenize the path, this time, taking care to properly
	 * handle .. and . to represent up (stack pop) and current
	 * (do nothing)
	 */
	
	// foreach(i, out){
	// 	printf("%s\n",i->val);
	// }


	while (pch != NULL) {
		if (!strcmp(pch,PATH_UP)) {
			/*
			 * Path = ..
			 * Pop the stack to move up a directory
			 */
			list_remove_back(out);


		} else if (!strcmp(pch,PATH_DOT)) {
			/*
			 * Path = .
			 * Do nothing
			 */
		} else {
			/*
			 * Regular path, push it
			 * XXX: Path elements should be checked for existence!
			 */
			char * s = malloc(sizeof(char) * (strlen(pch) + 1));
			memcpy(s, pch, strlen(pch) + 1);
			list_push(out, s);
		}
		pch = strtok_r(NULL, PATH_SEPARATOR_STRING, &save);
	}
	// free(path);

	// foreach(j, out){
	// 	printf("%s\n",j->val);
	// }

	/* Calculate the size of the path string */
	size_t size = 0;
	foreach(item, out) {
		/* Helpful use of our foreach macro. */
		size += strlen(item->val) + 1;
	}


	/* join() the list */
	char *output = malloc(sizeof(char) * (size + 1));
	char *output_offset = output;
	if (size == 0) {
		/*
		 * If the path is empty, we take this to mean the root
		 * thus we synthesize a path of "/" to return.
		 */
		output = realloc(output, sizeof(char) * 2);
		output[0] = PATH_SEPARATOR;
		output[1] = '\0';
	} else {
		/* Otherwise, append each element together */
		foreach(item, out) {
			output_offset[0] = PATH_SEPARATOR;
			output_offset++;
			memcpy(output_offset, item->val, strlen(item->val) + 1);
			output_offset += strlen(item->val);
		}
	}

	/* Clean up the various things we used to get here */
	list_destroy(out);
	free(out);

	/* And return a working, absolute path */
	return output;
}
