/*
   Word-frequencies-in-text-files-using-C

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct _{
      char word[100];
      unsigned long long int count;
      struct  _*next;

}node;

node *head=NULL; // to store the address of the first node
node *current; // a temporary variable to keep the pointer to the current node
unsigned long long int wordCount=0 ,fileLen=0;



void printUsage(char programName[]);//function that print usage of the program
int string2int(char *input);//convert string to int
void readChar(char *data);//this function read the characters of char array
void readWord(char *data);//this function read the words of char array also pre process word
void frequencyChar(char letter);//this function check if the character is exist in a present node.If so increment the count.
                           //If not exist It create a new node for the character
void frequencyWord(char *word);//this function check if the word is exist in a present node.If so increment the count.
                           //If not exist It create a new node for the word

//functions for sorting
node *mergeSortedList(node *front, node *back);//merge sorted two lists
void HeadTailSplit(node** frontP, node** backP,node* address);
void MergeSort(node **headP);//sorting function.

void plotChart(int rows,char mode,int scale);//function to plot the chart

int main(int argc,char *argv[]){
         //argument handling
         char programName[100];//array to store program name
         strcpy(programName,argv[0]);//copy program name in argv[0] to string array program

         //default values
         /*
         mode of frequency=words
         output rows=10
         */
         int rows=10;//default value for number of rows in the chart
         char freqMode='w';//default mode of frequency words
         int scaleMode=0;//default Scaled option is non scaled."1" for scaled mode


         //only one argument
                if(argc==1){
                   printf("No input files were given\n");
                   printUsage(programName);
                   return 0;
                }

                else if(argc>1){
                        char tempStore[100];//to store each argument

                        //int fileCount=0;//count number of files

                        int wordMode=0,CharMode=0;//this is used to check whether both -c and -w flags are entered

                        //check for flags
                        for(int count=0;count<argc;count++){//check each argument
                            strcpy(tempStore,argv[count]);//copy argument to a array

                            //check for Control arguments starting with "-"
                            if(tempStore[0]=='-'){
                                //check for frequency mode
                                if(tempStore[1]=='w'){
                                    wordMode++;
                                    freqMode='w';
                                }
                                else if(tempStore[1]=='c'){
                                    CharMode++;
                                    freqMode='c';
                                }
                                //check for scale mode

                                else if(tempStore[1]=='-'){//second index in '-'

                                       if(tempStore[2]=='s'&&tempStore[3]=='c'&&tempStore[4]=='a'&&tempStore[5]=='l'&&tempStore[6]=='e'&&tempStore[7]=='d' &&strlen(tempStore)==8){
                                            scaleMode=1;
                                        }
                                       else{//scaled is not correctly entered
                                           printf("Invalid option [%s]\n",tempStore);
                                           printUsage(programName);
                                           return 0;
                                       }

                                }




                                //check for number of rows
                                else if(tempStore[1]=='l'){
                                    //check if the -l is the last argument
                                    if(count==argc-1){
                                        printf("Not enough options for [-l]\n");
                                        printUsage(programName);
                                        return 0;
                                    }

                                    //check if there is a int value after 'l'
                                    else if(atoi(argv[count+1])!=0){

                                        int strinInt=string2int(argv[count+1]);

                                        if(strinInt==-1){
                                           printf("Invalid option(negative) for [-l]\n");
                                           printUsage(programName);
                                           return 0;
                                        }
                                        else if(strinInt==-2){
                                           printf("Invalid options for [-l]\n");
                                           printUsage(programName);
                                           return 0;
                                        }
                                        else rows=strinInt;


                                    }

                                    else{//value after l is not int
                                         printf("Invalid options for [-l]\n");
                                         printUsage(programName);
                                         return 0;

                                    }
                                }
                               else{//argument is not valid
                                   printf("Invalid option [%s]\n",tempStore);
                                   printUsage(programName);
                                   return 0;
                               }

                            }



                        }

                        //check if both -c and -w entered
                                if(wordMode>0 && CharMode>0){
                                          printf("[-c] and [-w] cannot use together\n");
                                          printUsage(programName);
                                          return 0;
                                }




                        //check for file names
                        for(int count=0;count<argc;count++){
                            strcpy(tempStore,argv[count]);//copy argument to a array

                            //file names are not start with a "-"
                            if(tempStore[0]!='-'){
                                  if(atoi(argv[count])!=rows && count!=0 ){

                                    //printf("File %i is %s\n",fileCount,argv[count]);

                                    FILE *fp,*checkFp;//pointer to file and checking file sizes
                                    fp=fopen(argv[count],"r");//open the file
                                    checkFp=fopen(argv[count],"r");//open the file

                                    //checking the file size
                                    fseek(checkFp,0L,SEEK_END);
                                    fileLen+=ftell(checkFp);

                                    //Cannot open file
                                    if(fp==NULL){
                                        printf("Cannot open one or more given files\n");
                                        return 0;
                                    }
                                    else if(freqMode=='c'){//letter mode
                                        //file opened

                                        //array to store line
                                        char data[2000]={0};

                                        //read the file
                                        while(fgets(data,2000,fp)!=NULL){
                                            //current line is loaded to the data array
                                            readChar(data);//this will read data array

                                        }


                                    }

                                     else if(freqMode=='w'){//word mode

                                          //array to store line
                                          char data[2000]={0};

                                          //read the file
                                          while(fgets(data,2000,fp)!=NULL){
                                                readWord(data);

                                          }

                                     }


                                 fclose(fp);




                                }


                            }

                        }


                }

                //If all files are empty files
                if(fileLen==0){
                    printf("No data to process\n");
                    return 0;
                }

                //sort linked list
                MergeSort(&head);

                plotChart(rows,freqMode,scaleMode);


     return 0;
}

void printUsage(char programname[]){
                printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",programname);
}

int string2int(char *input){
                int i;
                int sum=0;

                //check if this a decimal number
                for(i=0;input[i]!='\0';i++){
                        if(input[i]=='.')return -2;
                    }

                //If it is not a decimal number then check is this a negative number
                if(input[0]=='-') return -1;

                //positive number
                for(i=0;input[i]!='\0';i++){
                        int num=(int)input[i]-48;
                        sum=sum*10+num;
                }

                return sum;

}

void readChar(char *data){
              int i;
              char c;

              for(i=0;i<strlen(data);i++){
                //check whether character is alphanumeric
                if(isalnum(data[i])){
                   //check whether character is an alphabet
                   if(isalpha(data[i])){
                      //make capital letters simple
                      if(isupper(data[i])){
                        //printf("%c",tolower(data[i]));
                        c=tolower(data[i]);
                        //strcpy(letter,&c);
                        frequencyChar(c);
                      }
                      else{
                        //printf("%c",data[i]);
                        c=data[i];
                        //strcpy(letter,&c);
                        frequencyChar(c);
                      }
                   }

                   else{//character is a number
                        //printf("%c",data[i]);
                        c=data[i];
                        //strcpy(letter,&c);
                        frequencyChar(c);

                   }

                }



              }
}

void readWord(char *data){
            int i,j=0;

            char temp[2000];//store preprocessed line
            strcpy(temp,data);


            for(i=0;temp[i]!='\0';++i){
                 // enter the loop if the character is not an alnum
                 //or not null character
                 while(!isalnum(temp[i]) && temp[i]!=' ' && temp[i]!='\0'){
                    for(j=i;temp[j]!='\0';++j){
                        //if j th element is not alnum
                        temp[j]=temp[j+1];
                    }
                    temp[j]='\0';//add null
                 }


                      //make capital letters simple and add to temp
                      if(isalpha(temp[i]) && isupper(temp[i])){
                         temp[i]=tolower(temp[i]);
                      }

            }


            //getting word by word from preprocessed string
            const char s[2]=" ";
            char *token;

            //get first token
            token=strtok(temp,s);

            //go though other tokens
            while(token!=NULL){
                frequencyWord(token);//pass the word to function

                token=strtok(NULL,s);//update token
            }




            //printf("%s\n",temp);

}

void frequencyChar(char letter){
        int check=0;//to check if the word is exist in a present node
        char temp[10];



        //accessing the linked list
        for(current=head;current!=NULL;current=current->next){
            //check if there is the matching word
            strcpy(temp,current->word);

            //printf("%s and %s val %i\n",temp,word,wordCompare(temp,word));
            if(letter==temp[0]){
               //word is matched
               check++;
               //printf("word matched\n");
               unsigned long long int newCount=current->count;
               newCount++;
               current->count=newCount;//increment count

               break;
            }

        }



        //matching letter not found.so creating a new node for the letter
        if(check==0){
            //printf("unmatched\n");
            current=(node*)malloc(sizeof(node));
            strcpy(temp,&letter);
            strcpy(current->word,temp);
            current->count=1;//make the count 1
            current->next=head;
            head=current;
        }




        wordCount++;
}

void frequencyWord(char *word){
       int check=0;//to check if the word is exist in a present node
        char temp[100];



        //accessing the linked list
        for(current=head;current!=NULL;current=current->next){
            //check if there is the matching word
            strcpy(temp,current->word);

            //printf("%s and %s val %i\n",temp,word,wordCompare(temp,word));
            if(strcmp(word,temp)==0){
               //word is matched
               check++;
               //printf("word matched\n");
               unsigned long long int newCount=current->count;
               newCount++;
               current->count=newCount;//increment count

               break;
            }

        }



        //matching letter not found.so creating a new node for the letter
        if(check==0){
            //printf("unmatched\n");
            current=(node*)malloc(sizeof(node));
            strcpy(temp,word);
            strcpy(current->word,temp);
            current->count=1;//make the count 1
            current->next=head;
            head=current;
        }




        wordCount++;

}

void MergeSort(node **headP){
    node *headSp = *headP;//address of address of head node
    node *front,*back;



    if ((headSp == NULL) || (headSp->next == NULL)) {//head is null or second node is null
        return;
    }

    HeadTailSplit(&front,&back,headSp);//split head  in to two

    //sort both of them
    MergeSort(&front);
    MergeSort(&back);

    //merge after sorting
    *headP = mergeSortedList(front, back);
}

void HeadTailSplit(node** frontP, node** backP,node* address){
    node *a,*b;

    b = address;
    a = address->next;

    while (a != NULL) {
        a = a->next;
        if (a != NULL) {
            b = b->next;
            a = a->next;
        }
    }


    *frontP = address;

    *backP = b->next;

     b->next = NULL;
}

node *mergeSortedList(node *front, node *back){
     node *iter=NULL;

     //if one of this two node is null
    if (front == NULL) return (back);
    else if (back == NULL) return (front);

    //check for the node containing largest count of these two nodes
    if(front->count > back->count){
        iter = front;
        iter->next = mergeSortedList(front->next,back);
    }
    else{
        iter = back;
        iter->next = mergeSortedList(front, back->next);
    }
    return (iter);


}

void plotChart(int rows,char mode,int scale){
          //calculate the height of the chart
          int rightShift=0, maxCount=0,maxBar=0;
          const int maxWidth=80;//max width of the chart
          double precentage=0;
          int numberWidth=0;
          float barWidthDecimal=0;
          int barWidth=0;

          //calculate the right shift of the chart
          if(mode=='c') rightShift=3;
          else{//w mode
              int maxWord=0;//length of the largest word in given rows
              int printCount=0;
              for(current = head; current != NULL; current = current -> next){
                        if(strlen(current->word)>maxWord) maxWord=strlen(current->word);
                        printCount++;
                        if(rows==printCount) break;
                  }
              rightShift=2+maxWord;
          }

             //calculate max bar length
             current=head;
             precentage=(double)current -> count/ wordCount;


             maxCount=current -> count;//maximum word count

             if(precentage*100==100) numberWidth=6;
             else if(precentage*100>=10.0)numberWidth=5;
             else numberWidth=4;


             maxBar=maxWidth-rightShift-numberWidth-2;
             //if(mode=='w')maxBar--;
             //printf("maxBar %i numberWidth %i\n",maxBar,numberWidth);






          //print chart for each word
          printf("\n");
          int printCount=0;
          for(current = head; current != NULL; current = current -> next){
                        int spaces;//number of spaces
                        spaces=rightShift-1-strlen(current->word);

                        if(scale==1){//scaled mode
                        barWidthDecimal=maxBar*(current->count/(float)maxCount);
                        barWidth=(int)barWidthDecimal;
                        }
                        else{//non scaled mode
                        barWidthDecimal=maxBar*(current->count/(float)wordCount);
                        barWidth=(int)barWidthDecimal;
                        }


                        precentage=(double)current -> count/ wordCount;

                        for(int i=1;i<=4;i++){
                            if(i==2){//the line containing word
                                printf(" %s",current->word);
                                //print remaining spaces
                                for(int j=1;j<=spaces;j++) printf(" ");
                            }
                            else{
                                for(int j=1;j<=rightShift;j++) printf(" ");
                            }
                            //print u2502 character
                            printf("\u2502");



                            if(i!=4){//bar is printing for 3 lines
                                for(int j=1;j<=barWidth;j++) printf("\u2591");
                            }

                            //end of the 2nd row print precentage value

                            if(i==2){

                                char c='%';
                                printf("%.2lf%c",precentage*100,c);
                            }



                            printf("\n");

                        }

                        printCount++;
                        if(rows==printCount) break;
                  }

           //print last line
           for(int j=1;j<=rightShift;j++) printf(" ");
           printf("\u2514");
           for(int i=1;i<maxWidth-rightShift;i++){
               printf("\u2500");
           }
           printf("\n");




}


