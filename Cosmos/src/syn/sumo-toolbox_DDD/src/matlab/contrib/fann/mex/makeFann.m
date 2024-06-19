disp('Compiling the FANN mex interface (requires the fann library to be installed)..');

PathToYourLibrary = '''; % Write your library path
PathToYourIncludeFile = '''; % Write your include files

setenv('LIBRARY_PATH', [getenv('LIBRARY_PATH') ':' PathToYourLibrary]);
setenv('C_INCLUDE_PATH', [getenv('C_INCLUDE_PATH') ':'
PathToYourIncludeFile]);

mex CFLAGS#"-D_GNU_SOURCE -fPIC -pthread -m32  -fexceptions -D_FILE_OFFSET_BITS=64 -Wall -fPIC -O3" -lm -lfann -c helperFann.c
mex CFLAGS#"-D_GNU_SOURCE -fPIC -pthread -m32  -fexceptions -D_FILE_OFFSET_BITS=64 -Wall -fPIC -O3" -lm -lfann createFann.c helperFann.o
mex CFLAGS#"-D_GNU_SOURCE -fPIC -pthread -m32  -fexceptions -D_FILE_OFFSET_BITS=64 -Wall -fPIC -O3" -lm -lfann trainFann.c helperFann.o
mex CFLAGS#"-D_GNU_SOURCE -fPIC -pthread -m32  -fexceptions -D_FILE_OFFSET_BITS=64 -Wall -fPIC -O3" -lm -lfann testFann.c helperFann.o

% Orig code
%mex -O CC#gcc CFLAGS#"-Wall -fPIC -O3" -lm -lfann -c helperFann.c
%mex -O CC#gcc CFLAGS#"-Wall -fPIC -O3" -lm -lfann createFann.c helperFann.o
%mex -O CC#gcc CFLAGS#"-Wall -fPIC -O3" -lm -lfann trainFann.c helperFann.o
%mex -O CC#gcc CFLAGS#"-Wall -fPIC -O3" -lm -lfann testFann.c helperFann.o
