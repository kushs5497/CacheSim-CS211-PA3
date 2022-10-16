#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int tagbits;
int setbits;
int offset;
int addressLength = 48;

int logTwo(int x){
    int result=0;
    for(int i=x;i>1;i/=2) result++;
    return result;
}

long long int getTag(long long int memAddy){
    int x =setbits+offset;
    return memAddy>>(x);
}

long long int getSet(long long int memAddy){
    memAddy &= (((1<<setbits)-1)<<(offset));
    return memAddy>>offset;
}

int main(int argc, char *argv[argc+1]){
    int way;
    if(strcmp(argv[2],"direct")==0) way = 1;
    else if(strcmp(argv[2],"assoc")==0) way = atoi(argv[1])/atoi(argv[3]);
    else{
        char assoc[strlen(argv[2])-6+1];
        strncpy(assoc,&argv[2][6],strlen(argv[2])-6);
        way = atoi(assoc);
    }
    int sets=atoi(argv[1])/atoi(argv[3])/way;
    offset=logTwo(atoi(argv[3]));
    setbits=logTwo(sets);
    tagbits=addressLength-(offset+setbits);
    long long int cache[sets][way+1];
    for(int i=0;i<sets;i++) cache[i][way]=0;
    FILE * fp = fopen(argv[4],"r");
    char RorW='R';
    int hits=0;
    int writes=0;
    long long int memAddy;
    long long int c=0;
    while(fscanf(fp," %c %llx",&RorW,&memAddy)!=EOF){
        c++;
        long long int t = getTag(memAddy);
        long long int s = getSet(memAddy);
        if(RorW=='W') writes++;
        int noHit=1;
        for(int i=0;i<way;i++){
            if(t==getTag(cache[s][i])){
                hits++;
                noHit=0;
                break;
            }
        }
        if(noHit==1){
            cache[s][cache[s][way]]= memAddy;
            cache[s][way]=(cache[s][way]+1)%way;
        }        
    }
    printf("memread:%lld\nmemwrite:%d\ncachehit:%d\ncachemiss:%lld\n",c-hits,writes,hits,c-hits);
    return EXIT_SUCCESS;
}
//create a method to calculate tag size, setbit size, #sets, & offset bits
// ^^ calculate this given cache size, block size, and ways
//create a method to get the tag bits
//create a method to get the set bits
//create a method to get the offset bits
//create a method to search your cashtable to determine hits and misses
//if write & hit: write++, hit++
//if write & miss: write++, miss++, read++
//if read & hit: hit++
//if read & miss: read++, hit++