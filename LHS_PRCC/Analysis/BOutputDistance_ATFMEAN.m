% THIS code is for the PRCC calculation for each parameters
% Depends on the number of runs 

clear
mkdir 'Z.Distance_ATFMEAN'
%%% THIS READS THE LHS MATRIX FILE %%%
LHS_Raw=importdata('/Users/debjit/Documents/ABM/4.LHS_PRCC/ABM_DR/my.txt');
LHS_FINAL=LHS_Raw.data';

%%% ARRAY FOR GETTING THE OUTPUT VARIABLES %%%
UNDIFF_DIST=[];
DIFF_DIST=[];
PRELEP_DIST=[];
LEP_DIST=[];
ZYG_DIST=[];
PACHY_DIST=[];
DIP_DIST=[];
SEC_DIST=[];
ROUND_DIST=[];
ELONG_DIST=[];


% DEPENDENT ON NUMBER OF RUNS
RUNS=300;

% ALL AVERAGE NUMBERS FOR THE 10 DIFFERENT CELL TYPES
for set=0:RUNS-1  
    set
    % FOR UNDIFF
    c1=['/Users/debjit/Desktop/New/LHS_TEST/SET_',num2str(set),'/Results_Stats/0.Output_2/11.Undiff_Dist.txt'];
    W1=importdata(c1);
    UNDIFF_DIST=[UNDIFF_DIST;W1.data(:,3)'];
    % FOR DIFF
    c2=['/Users/debjit/Desktop/New/LHS_TEST/SET_',num2str(set),'/Results_Stats/0.Output_2/12.Diff_Dist.txt'];
    W2=importdata(c2);
    DIFF_DIST=[DIFF_DIST;W2.data(:,3)'];
    % FOR PRELEP
    c3=['/Users/debjit/Desktop/New/LHS_TEST/SET_',num2str(set),'/Results_Stats/0.Output_2/13.Prelep_Dist.txt'];
    W3=importdata(c3);
    PRELEP_DIST=[PRELEP_DIST;W3.data(:,3)'];
    % FOR LEP
    c4=['/Users/debjit/Desktop/New/LHS_TEST/SET_',num2str(set),'/Results_Stats/0.Output_2/14.Lep_Dist.txt'];
    W4=importdata(c4);
    LEP_DIST=[LEP_DIST;W4.data(:,3)'];
    % FOR ZYG
    c5=['/Users/debjit/Desktop/New/LHS_TEST/SET_',num2str(set),'/Results_Stats/0.Output_2/15.Zyg_Dist.txt'];
    W5=importdata(c5);
    ZYG_DIST=[ZYG_DIST;W5.data(:,3)'];
    % FOR PACHY
    c6=['/Users/debjit/Desktop/New/LHS_TEST/SET_',num2str(set),'/Results_Stats/0.Output_2/16.Pachy_Dist.txt'];
    W6=importdata(c6);
    PACHY_DIST=[PACHY_DIST;W6.data(:,3)'];
    % FOR DIP
    c7=['/Users/debjit/Desktop/New/LHS_TEST/SET_',num2str(set),'/Results_Stats/0.Output_2/17.Dip_Dist.txt'];
    W7=importdata(c7);
    DIP_DIST=[DIP_DIST;W7.data(:,3)'];
    % FOR SEC
    c8=['/Users/debjit/Desktop/New/LHS_TEST/SET_',num2str(set),'/Results_Stats/0.Output_2/18.Sec_Dist.txt'];
    W8=importdata(c8);
    SEC_DIST=[SEC_DIST;W8.data(:,3)'];
    % FOR ROUND
    c9=['/Users/debjit/Desktop/New/LHS_TEST/SET_',num2str(set),'/Results_Stats/0.Output_2/19.Round_Dist.txt'];
    W9=importdata(c9);
    ROUND_DIST=[ROUND_DIST;W9.data(:,3)'];
    % FOR ELONG
    c10=['/Users/debjit/Desktop/New/LHS_TEST/SET_',num2str(set),'/Results_Stats/0.Output_2/20.Elong_Dist.txt'];
    W10=importdata(c10);
    ELONG_DIST=[ELONG_DIST;W10.data(:,3)'];

end

VALIDSIZE=[];

%for time= [1336,1366,1380,1397,1404,1425,1446,1470,1488,1506,1524]
for MAIN=round([4.01:8.61:72.89]*24)

    CUMM_UNDIFF=[];
    CUMM_DIFF=[];
    CUMM_PRELEP=[];
    CUMM_LEP=[];
    CUMM_ZYG=[];
    CUMM_PACHY=[];
    CUMM_DIP=[];
    CUMM_SEC=[];
    CUMM_ROUND=[];
    CUMM_ELONG=[];


    for time= [MAIN:round(MAIN+(8.61*24)-1)]

        time
        UNDIFF=UNDIFF_DIST(:,time)';
        DIFF=DIFF_DIST(:,time)';
        PRELEP=PRELEP_DIST(:,time)';
        LEP=LEP_DIST(:,time)';
        ZYG=ZYG_DIST(:,time)';
        PACHY=PACHY_DIST(:,time)';
        DIP=DIP_DIST(:,time)';
        SEC=SEC_DIST(:,time)';
        ROUND=ROUND_DIST(:,time)';
        ELONG=ELONG_DIST(:,time)';



        CUMM_UNDIFF=[CUMM_UNDIFF;UNDIFF];
        CUMM_DIFF=[CUMM_DIFF;DIFF];
        CUMM_PRELEP=[CUMM_PRELEP;PRELEP];
        CUMM_LEP=[CUMM_LEP;LEP];
        CUMM_ZYG=[CUMM_ZYG;ZYG];
        CUMM_PACHY=[CUMM_PACHY;PACHY];
        CUMM_DIP=[CUMM_DIP;DIP];
        CUMM_SEC=[CUMM_SEC;SEC];
        CUMM_ROUND=[CUMM_ROUND;ROUND];
        CUMM_ELONG=[CUMM_ELONG;ELONG];

    end



    MEAN_UNDIFF=nanmean(CUMM_UNDIFF)';
    MEAN_DIFF=nanmean(CUMM_DIFF)';
    MEAN_PRELEP=nanmean(CUMM_PRELEP)';
    MEAN_LEP=nanmean(CUMM_LEP)';
    MEAN_ZYG=nanmean(CUMM_ZYG)';
    MEAN_PACHY=nanmean(CUMM_PACHY)';
    MEAN_DIP=nanmean(CUMM_DIP)';
    MEAN_SEC=nanmean(CUMM_SEC)';
    MEAN_ROUND=nanmean(CUMM_ROUND)';
    MEAN_ELONG=nanmean(CUMM_ELONG)';


    Names={'UNDIFFERENTIATED_SPERMATOGONIA' 'DIFFERENTIATED_SPERMATOGONIA' 'PRELEPTOTENE' 'LEPTOTENE' 'ZYGOTENE' 'PACHYTENE' 'DIPLOTENE' 'SECOND_SPERMATOCTYE' 'ROUND_SPERMATIDS' 'ELONGATED_SPERMATIDS'}';
    PRCC_vari= {'DIFFERENTIATED_SPERMATOGONIA_DIFFERENTIATION    ' 'PRELEPTOTENE_DIFFERENTIATION    ' 'LEPTOTENE_DIFFERENTIATION    ' 'ZYGOTENE_DIFFERENTIATION    ' 'PACHYTENE_DIFFERENTIATION    ' 'ROUND_SPERMATIDS_DIFFERENTIATION    ' 'DIFFERENTIATED_SPERMATOGONIA_PROLIFERATION    ' 'DIPLOTENE_PROLIFERATION    ' 'SECOND_SPERMATOCTYE_PROLIFERATION    ' 'DIFFERENTIATED_SPERMATOGONIA_APOPTOSIS_MIN    ' 'PRELEPTOTENE_APOPTOSIS_MIN    ' 'LEPTOTENE_APOPTOSIS_MIN    ' 'ZYGOTENE_APOPTOSIS_MIN    ' 'PACHYTENE_APOPTOSIS_MIN    ' 'DIPLOTENE_APOPTOSIS_MIN    ' 'SECOND_SPERMATOCTYE_APOPTOSIS_MIN    ' 'ROUND_SPERMATIDS_APOPTOSIS_MIN    ' 'ELONGATED_SPERMATIDS_APOPTOSIS_MIN    ' 'UNDIFFERENTIATED_SPERMATOGONIA_SPEED    ' 'DIFFERENTIATED_SPERMATOGONIA_SPEED    ' 'PRELEPTOTENE_SPEED    ' 'LEPTOTENE_SPEED    ' 'ZYGOTENE_SPEED    ' 'PACHYTENE_SPEED    ' 'DIPLOTENE_SPEED    ' 'SECOND_SPERMATOCTYE_SPEED    ' 'ROUND_SPERMATIDS_SPEED    ' 'ELONGATED_SPERMATIDS_SPEED    ' 'UNDIFFERENTIATED_SPERMATOGONIA_INITIAL_NUM '}';


    [prcc1 sign1 sign_label1]=PRCC1(LHS_FINAL(find(~isnan(MEAN_UNDIFF)),:),MEAN_UNDIFF(find(~isnan(MEAN_UNDIFF))),time,PRCC_vari,0.05);

    [prcc2 sign2 sign_label2]=PRCC1(LHS_FINAL(find(~isnan(MEAN_DIFF)),:),MEAN_DIFF(find(~isnan(MEAN_DIFF))),time,PRCC_vari,0.05);

    [prcc3 sign3 sign_label3]=PRCC1(LHS_FINAL(find(~isnan(MEAN_PRELEP)),:),MEAN_PRELEP(find(~isnan(MEAN_PRELEP))),time,PRCC_vari,0.05);

    [prcc4 sign4 sign_label4]=PRCC1(LHS_FINAL(find(~isnan(MEAN_LEP)),:),MEAN_LEP(find(~isnan(MEAN_LEP))),time,PRCC_vari,0.05);

    [prcc5 sign5 sign_label5]=PRCC1(LHS_FINAL(find(~isnan(MEAN_ZYG)),:),MEAN_ZYG(find(~isnan(MEAN_ZYG))),time,PRCC_vari,0.05);

    [prcc6 sign6 sign_label6]=PRCC1(LHS_FINAL(find(~isnan(MEAN_PACHY)),:),MEAN_PACHY(find(~isnan(MEAN_PACHY))),time,PRCC_vari,0.05);

    [prcc7 sign7 sign_label7]=PRCC1(LHS_FINAL(find(~isnan(MEAN_DIP)),:),MEAN_DIP(find(~isnan(MEAN_DIP))),time,PRCC_vari,0.05);

    [prcc8 sign8 sign_label8]=PRCC1(LHS_FINAL(find(~isnan(MEAN_SEC)),:),MEAN_SEC(find(~isnan(MEAN_SEC))),time,PRCC_vari,0.05);

    [prcc9 sign9 sign_label9]=PRCC1(LHS_FINAL(find(~isnan(MEAN_ROUND)),:),MEAN_ROUND(find(~isnan(MEAN_ROUND))),time,PRCC_vari,0.05);

    [prcc10 sign10 sign_label10]=PRCC1(LHS_FINAL(find(~isnan(MEAN_ELONG)),:),MEAN_ELONG(find(~isnan(MEAN_ELONG))),time,PRCC_vari,0.05);

    
    
    TOTAL=[size(MEAN_UNDIFF(find(~isnan(MEAN_UNDIFF))),1),size(MEAN_DIFF(find(~isnan(MEAN_DIFF))),1),size(MEAN_PRELEP(find(~isnan(MEAN_PRELEP))),1),size(MEAN_LEP(find(~isnan(MEAN_LEP))),1),size(MEAN_ZYG(find(~isnan(MEAN_ZYG))),1),size(MEAN_PACHY(find(~isnan(MEAN_PACHY))),1),size(MEAN_DIP(find(~isnan(MEAN_DIP))),1),size(MEAN_SEC(find(~isnan(MEAN_SEC))),1),size(MEAN_ROUND(find(~isnan(MEAN_ROUND))),1),size(MEAN_ELONG(find(~isnan(MEAN_ELONG))),1)];

    time=MAIN;
    % PRCC DATA FOR 10 CELL TYPES
    prcc=[prcc1' prcc2' prcc3' prcc4' prcc5' prcc6' prcc7' prcc8' prcc9' prcc10']';
    prcc=num2cell(prcc);
    k=repmat('%.3f\t',1,RUNS);
    fileName = ['Z.Distance_ATFMEAN/X.PRCC_' num2str(time) '.txt'];
    fid=fopen(fileName,'w');
    for row =1:length(Names)
    
        fprintf(fid,'%s\t',Names{row,:});
        fprintf(fid,k, prcc{row,:});
        fprintf(fid,'\n');
    end
    fclose(fid);


    % SIGN DATA FOR 10 CELL TYPES
    sign=[sign1' sign2' sign3' sign4' sign5' sign6' sign7' sign8' sign9' sign10']';
    sign=num2cell(sign);
    k=repmat('%.3f\t',1,RUNS);
    fileName = ['Z.Distance_ATFMEAN/Y.SIGNS_' num2str(time) '.txt']; 
    fid=fopen(fileName,'w');
    for row =1:length(Names)
    
        fprintf(fid,'%s\t',Names{row,:});
        fprintf(fid,k, sign{row,:});
        fprintf(fid,'\n');
    end
    fclose(fid);
 
 
    % SAVING THE DATA FOR THE LOW P-VALUES AND PRCC LABELS FOR 10 CELL TYPES
    for i=1:10 
     
        INDEX=['sign_label' num2str(i) '.index{1}'];
        SIGN_VALUES=['sign_label' num2str(i) '.uncorrected_sign(sign_label' num2str(i) '.index{1})'];
        PRCC_VALUES=['prcc' num2str(i) '(sign_label' num2str(i) '.index{1})'];
        LABELS=['sign_label' num2str(i) '.label{1}'];

        %dlmwrite(['Z.Distance_ATFMEAN/Z.Summ_' num2str(i) '_' num2str(time) '.txt'],eval(INDEX),'delimiter', '\t');
        %dlmwrite(['Z.Distance_ATFMEAN/Z.Summ_' num2str(i) '_' num2str(time) '.txt'],eval(SIGN_VALUES),'-append','delimiter', '\t','precision', 2);
        %dlmwrite(['Z.Distance_ATFMEAN/Z.Summ_' num2str(i) '_' num2str(time) '.txt'],eval(PRCC_VALUES),'-append','delimiter', '\t','precision', 2);
        %dlmwrite(['Z.Distance_ATFMEAN/Z.Summ_' num2str(i) '_' num2str(time) '.txt'],eval(LABELS)','-append','delimiter', '');
     
    end
    
    VALIDSIZE=[VALIDSIZE;TOTAL];
    
    if (MAIN==1129) 
        PRCC_PLOT(LHS_FINAL(find(~isnan(MEAN_UNDIFF)),:),MEAN_UNDIFF(find(~isnan(MEAN_UNDIFF))),1129,PRCC_vari,'UNDIFF');
        PRCC_PLOT(LHS_FINAL(find(~isnan(MEAN_DIFF)),:),MEAN_DIFF(find(~isnan(MEAN_DIFF))),1129,PRCC_vari,'DIFF');
        PRCC_PLOT(LHS_FINAL(find(~isnan(MEAN_PRELEP)),:),MEAN_PRELEP(find(~isnan(MEAN_PRELEP))),1129,PRCC_vari,'PRELEP');
        PRCC_PLOT(LHS_FINAL(find(~isnan(MEAN_LEP)),:),MEAN_LEP(find(~isnan(MEAN_LEP))),1129,PRCC_vari,'LEP');
        PRCC_PLOT(LHS_FINAL(find(~isnan(MEAN_ZYG)),:),MEAN_ZYG(find(~isnan(MEAN_ZYG))),1129,PRCC_vari,'ZYG');
        PRCC_PLOT(LHS_FINAL(find(~isnan(MEAN_PACHY)),:),MEAN_PACHY(find(~isnan(MEAN_PACHY))),1129,PRCC_vari,'PACHY');
        PRCC_PLOT(LHS_FINAL(find(~isnan(MEAN_DIP)),:),MEAN_DIP(find(~isnan(MEAN_DIP))),1129,PRCC_vari,'DIP');
        PRCC_PLOT(LHS_FINAL(find(~isnan(MEAN_SEC)),:),MEAN_SEC(find(~isnan(MEAN_SEC))),1129,PRCC_vari,'SEC');
        PRCC_PLOT(LHS_FINAL(find(~isnan(MEAN_ROUND)),:),MEAN_ROUND(find(~isnan(MEAN_ROUND))),1129,PRCC_vari,'ROUND');
        PRCC_PLOT(LHS_FINAL(find(~isnan(MEAN_ELONG)),:),MEAN_ELONG(find(~isnan(MEAN_ELONG))),1129,PRCC_vari,'ELONG');
    end

end
CYCLES=round([4.01:8.61:72.89]*24);

VALIDSIZE=[CYCLES;VALIDSIZE']';

dlmwrite('Z.Distance_ATFMEAN/0.VALIDSIZE.csv',VALIDSIZE);



















