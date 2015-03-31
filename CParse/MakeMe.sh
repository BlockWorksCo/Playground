echo "#define __extension__" > t3.c
cpp t2.c >> t3.c
python ctoc.py t3.c
