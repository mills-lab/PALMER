//copyright by ArthurZhou @ UMich&FUDAN&HUST
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <cstdlib>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "samtools.cpp"
#include "rm_selector.cpp"
#include "read_masker.cpp"
#include "blastn.cpp"
#include "blastn_caller.cpp"
#include "TSD_seq.cpp"
#include "calling.cpp"
using namespace std;

int tube(string working_dir, string input_bam, string chr, int start, int end){
    
//building working directory
    int start1, end1;
    start1= start;
    end1 = end;
    stringstream ss1, ss2;
    ss1 << start1;
    string s_start =ss1.str();
    ss2 << end1;
    string s_end =ss2.str();
    
    string WD_tube;
    WD_tube=working_dir+chr+"_"+s_start+"_"+s_end+"/";
    
    string sys_mkdir="mkdir "+WD_tube;
    
    cout<<"Working in the direcotry "<<sys_mkdir<<"."<<endl;
    
    char *syst_mkdir =new char[sys_mkdir.length()+1];
    strcpy(syst_mkdir, sys_mkdir.c_str());
    system(syst_mkdir);
 
//samtools view
    
    samtools(working_dir, WD_tube, input_bam, chr, s_start, s_end);
    //cout<<"1. Samtools Step for region "+chr+"_"+s_start+"_"+s_end+" is now completed."<<endl;
    
//Repeat region output
    
    ofstream file1;
    
    string sys_RMloc = WD_tube+"RM.loc";
    char *syst_RMloc = new char[sys_RMloc.length()+1];
    strcpy(syst_RMloc, sys_RMloc.c_str());
    
    file1.open(syst_RMloc);
    file1<<"chr"<<chr<<'\t'<<(start-100000)<<'\t'<<(end+100000)<<endl;

//Repeat region selector
    
    RMSelector(working_dir, WD_tube);
    //cout<<"2. RMSelector Step for region "+chr+"_"+s_start+"_"+s_end+" is now completed."<<endl;
    
//Read masker
    
    ReadMasker(WD_tube);
    //cout<<"3. Read Masker Step for region "+chr+"_"+s_start+"_"+s_end+" is now completed."<<endl;
 
//Blastn
    
    blastn(working_dir, WD_tube);
    //cout<<"4. Blastn Step for region "+chr+"_"+s_start+"_"+s_end+" is now completed."<<endl;
    
//Blastn caller
    
    BlastnCaller(WD_tube, chr);
    //cout<<"5. Blastn Caller Step for region "+chr+"_"+s_start+"_"+s_end+" is now completed."<<endl;
 
    cout<<"Pre-masking step & single read calling step for "+chr+"_"+s_start+"_"+s_end+" completed."<<endl;

//TSD module
    
    tsd_module(WD_tube);
    cout<<"TSD_module step for "+chr+"_"+s_start+"_"+s_end+" completed."<<endl;
    
//calling module
    
    calling(WD_tube);
    cout<<"Calling step for "+chr+"_"+s_start+"_"+s_end+" completed."<<endl;
}