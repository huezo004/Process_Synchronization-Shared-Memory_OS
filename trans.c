#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>

int main(int argc, const char * argv[]){

   char answer[2]=""; 

   char n[2]= "N"; 

   if(argc!=3){
     fprintf(stderr, "Incorrect number of commands\n");    
     exit(EXIT_FAILURE);
   }

   //Check file existance
   if(access(argv[2], F_OK ) != -1 ) {
     
     printf("File exists, would you like to override the file? Enter Y/N\n"); 
     
     fgets(answer, 2, stdin); 
    
     if(strcmp(answer,n)==0){       
       exit(0); 
      }             
   }

   //File intizialitation and error handling    
   FILE *file1;

   FILE *file2;

   file1= fopen(argv[1],"rb");

   file2= fopen(argv[2],"wb");
 
   if(file1==NULL){
      fprintf(stderr, "File to be copied cannot be opened!\n");
      exit(1);
    }
   
   if(file2==NULL){
     fprintf(stderr, "File to write copy cannot be opened!\n");
     exit(1);
    }
  
   //Pipe initialization and error handling 
   
   int pipe1[2]; 

   int pipe2[2]; 
   
   if(pipe(pipe1)== -1){
      perror("Error in pipe creation");
      exit(EXIT_FAILURE);
    }
    
  if(pipe(pipe2)== -1){
     perror("Error in second pipe creation");
     exit(EXIT_FAILURE);   
   }
  
  int block[2]={100,4020};

  int parentReceive[2]={0,0}; 

  int childReceive[2]={0,0}; 

  pid_t pid = fork(); 

  if(pid == -1){
    perror("fork"); 
    exit(EXIT_FAILURE); 
   }
   
  if(pid>1){  // Parent Process 

    //Close pipe1 read 
     close(pipe1[0]); 

    //Close pipe2 write 
     close(pipe2[1]); 

     write(pipe1[1], &block, sizeof(block));   
 
     close(pipe1[1]); 
        
     wait(NULL);

     read(pipe2[0], &parentReceive, sizeof(parentReceive)); 

     printf("Parent received %d, %d\n", parentReceive[0], parentReceive[1]);  
               
   }

  if(pid == 0){  //Child Process  
   
    //Close pipe1 write 
     close(pipe1[1]); 
     
    //Close pipe2 read 
     close(pipe2[0]); 
 
     read(pipe1[0], &childReceive, sizeof(childReceive)); 
     
     close(pipe1[0]); 

     printf("Child received %d, %d\n", childReceive[0], childReceive[1]); 
          
     write(pipe2[1], &childReceive, sizeof(childReceive));     
    } 

 }// end main()

/* //~array with block info~      
 
  int blockInfo[2];
  int blockInfo2[2];
  
  //~Pipes~ 
  int pip1[2];
  int pip2[2]; 

  if(pipe(pip1)== -1){
    perror("Error in pipe creation");
    exit(1);
  }
    
  if(pipe(pip2)==-1){
    perror("Error in second pipe creation");
    exit(1);   
  }
  
  //~fork~ 
  pid_t pid;  
  pid=fork();
  if(pid== -1){
    perror("fork");
    exit(1); 
  }
  
  //~shared memory~ 
  const int SIZE=4096;

  const char *name= "dah12";

  //sdm file descriptor
  int shm_fd;

  //pointer to shard memory object 
  void *ptr;

  //create shared memory object 
  
  shm_fd= shm_open(name, O_CREAT | O_RDWR, 0666);

  /*
  if(shm_fd==-1){
    printf("Shared memory failed");
    exit(1);    
  }
  
  //configure size 
  ftruncate(shm_fd, SIZE);

  //memory map the shaed memory object 
    ptr= mmap(0,SIZE,PROT_WRITE|PROT_READ, MAP_SHARED, shm_fd, 0);

  //~child~
     if(pid==0){
     
      close(pip1[1]);// close output pd1    
      close(pip2[0]);// close input fd2
  
     //while(1){
           
    while(read(pip1[0], blockInfo, sizeof(blockInfo))> 0);   // read from pip1          

    fwrite(ptr,1,blockInfo[1],f2);  //write from shared memory to last file

    printf("%d", blockInfo[0]);
    printf("\n"); 

    blockInfo2[0]= blockInfo[0]; 
       
    write(pip2[1],blockInfo2, sizeof(blockInfo2));//send back block number trough pip2

    //}
    close(pip1[0]);
    close(pip2[1]);
   
   }
   
  //~parent~
    else{
    close(pip1[0]);//writing pd1
    close(pip2[1]);//reading pd2
    int counter=1;    
    size_t pread=0;
    size_t rread=0; 
    
    do{
      
      pread=fread(ptr,1, 4096, f1); //copy 4096  bytes from file to buf

      blockInfo[0]= counter; //passed #
           
      blockInfo[1]= pread;  //return from fread  
      
      write(pip1[1],blockInfo, sizeof(blockInfo)); //send block numbers to child
           
      while(rread=read(pip2[0],blockInfo2, sizeof(blockInfo2))>0);   //read from pipe2
      
      printf("BlockInfo2[0]:\n");
      
      printf("%d", blockInfo2[0]);
      
      if(blockInfo2[0]==counter)
	   counter++;

     }while(pread >= 4096);
    
    close(pip1[1]);
    close(pip2[0]);

    }//end parent 
   
  shm_unlink(name); //close memory
  //close(f1);
  //close(f2);
  // mmap(ptr);
 }//end main
  
  */ 
