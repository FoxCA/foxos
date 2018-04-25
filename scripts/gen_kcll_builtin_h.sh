#!/bin/sh
_gen_bnb () {                                                                                                              
  bnz="$1"                                                                                                                 
  bnx="$2"                                                                                                                 
  bna=${bnz%$bnx}                                                                                                          
  bnb=${bna##*/}                                                                                                           
  echo "$bnb"                                                                                                              
}
rm "src/arch/x86_64/os/kcll/include/kcll_builtin.h"
echo "#ifdef KCLL_BUILTIN_H" > "src/arch/x86_64/os/kcll/include/kcll_builtin.h"
echo "#define KCLL_BUILTIN_H" >> "src/arch/x86_64/os/kcll/include/kcll_builtin.h"
for KCLL_BUILTIN in src/arch/x86_64/os/kcll/builtin/include/*; do
  echo "$KCLL_BUILTIN"
  if [ "$KCLL_BUILTIN" != "$(_gen_bnb "include.h" .h).h" ]; then
    echo "#include <$(_gen_bnb $KCLL_BUILTIN .h).h>" >> "src/arch/x86_64/os/kcll/include/kcll_builtin.h"
  fi
done
echo "#endif" >> "src/arch/x86_64/os/kcll/include/kcll_builtin.h"
