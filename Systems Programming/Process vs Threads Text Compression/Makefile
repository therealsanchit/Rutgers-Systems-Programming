all : main lzw extra

main : compressT_LOLS.c compressR_LOLS.c 	
	gcc -o threadCompressor compressT_LOLS.c -lm -pthread	
	gcc -o processCompressor compressR_LOLS.c -lm	

lzw: compressR_LZW.c compressT_LZW.c	
	gcc -o lzwThreadCompressor compressT_LZW.c -lm -pthread	
	gcc -o lzwProcessCompressor compressR_LZW.c -lm				

extra : compressR_worker_LOLS.c decoder.c compressR_worker_LZW.c 
	gcc -o lolsChildProcess compressR_worker_LOLS.c -lm
	gcc -o lzwChildProcess compressR_worker_LZW.c -lm
	gcc -o lzwdecoder decoder.c -lm 
clean: 
	rm *_LOLS_*.txt 
	rm *_LZW*.txt
