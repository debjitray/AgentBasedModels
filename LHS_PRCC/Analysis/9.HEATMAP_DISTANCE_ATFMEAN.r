rm(list = ls())
library(Biobase)
library (Hmisc)
library(gplots)
library(gtools)

vari=c('Differentiated spermatogonia differentiation','Preleptotene differentiation','Leptotene differentiation','Zygotene differentiation','Pachytene differentiation','Round spermatids differentiation','Differentiated spermatogonia proliferation','Diplotene proliferation','Secondary spermatocyte proliferation','Differentiated spermatogonia apoptosis_min','Preleptotene apoptosis_min','Leptotene apoptosis_min','Zygotene apoptosis_min','Pachytene apoptosis_min','Diplotene apoptosis_min','Secondary spermatocyte apoptosis_min','Round spermatid apoptosis_min','Elongated spermatid apoptosis_min','Undifferentiated spermatogonia speed','Differentiated spermatogonia speed','Preleptotene speed','Leptotene speed','Zygotene speed','Pachytene speed','Diplotene speed','Secondary spermatocyte speed','Round spermtid speed','Elongated spermatid speed','Undifferentiated spermatogonia initial number');

CELL=c('Undifferentiated spermatogonia','Differentiated spermatogonia','Preleptotene','Leptotene','Zygotene','Pachytene','Diplotene','Secondary spermatocyte','Round spermatid','Elongated spermatid','Sperm');



TIMEFRAME=round(c(seq(4.01,72.89,8.61))*24);

for (i in 1:9) {
	PRCC <- read.table(paste('Z.Distance_ATFMEAN/X.PRCC_',TIMEFRAME[i],'.txt',sep=""), sep="\t",header=F);

	# PLOTS FOR P-VALUES
	SIGNS <- read.table(paste('Z.Distance_ATFMEAN/Y.SIGNS_',TIMEFRAME[i],'.txt',sep=""), sep="\t",header=F);

	# PICKING ONLY THE SIGNIFICANT PARAMETERS
	SIGNIFICANTS=c();
	for (joop in 1:10) {
		POP=as.matrix(SIGNS[joop,2:30]);
		for (u in 1:29) {
			if (POP[u] < 0.05) {
				SIGNIFICANTS=append(SIGNIFICANTS,u)
			}
		}
	}

	POX=c();
	for (joop in 1:10) {
		P1=as.matrix(PRCC[joop,2:30]);
		P2=P1;
		SIFF=as.matrix(SIGNS[joop,2:30]);
		P2[SIFF>=0.05]=NA;
		POX=rbind(POX,P2);
	}


	SIGNIFICANTS=sort(unique(SIGNIFICANTS));
	z=cbind(POX[,SIGNIFICANTS]);
	M=as.matrix(z);

	pdf(file=paste('Z.Distance_ATFMEAN/4.HeatMapDist_',TIMEFRAME[i],'.pdf',sep=""),width=30,height=20,pointsize=20);
	heatmap(M,Rowv=NA, Colv=NA, scale="none", revC=T,labRow=CELL,labCol=vari[SIGNIFICANTS], breaks=c(seq(-1, 1, 0.1)), col =colorRampPalette(c("navy", "white", "red"))(20),margins = c(22,15));
	dev.off();


#KOKA=cbind(seq(1,-1,-0.2),seq(1,-1,-0.2));
#KOKA=as.matrix(KOKA);
#heatmap(KOKA,Rowv=NA, Colv=NA, scale="none", revC=T,labRow=c(seq(1,-1,-0.2)),labCol=vari[SIGNIFICANTS], breaks=c(seq(-1, 1, 0.1)), col =colorRampPalette(c("navy", "white", "red"))(20),margins = c(22,15));



#####

	DOX=z;

	SORTED=sort(DOX);

	DOX[match(SORTED[1],DOX)]=SORTED[1]*(1000000);
	DOX[match(SORTED[2],DOX)]=SORTED[2]*(1000000);
	DOX[match(SORTED[length(SORTED)],DOX)]=SORTED[length(SORTED)]*1000000;
	DOX[match(SORTED[length(SORTED)-1],DOX)]=SORTED[length(SORTED)-1]*1000000;

	DOX[is.na(DOX)]<-1

	for (HOO in 1:length(DOX)) {
		if (abs(DOX[HOO]) > 100) {
			DOX[HOO]=DOX[HOO]/1000000;
		}
		else {
			DOX[HOO]=NA;
		}
	}

	pdf(file=paste('Z.Distance_ATFMEAN/0.VerifierDist_',TIMEFRAME[i],'.pdf',sep=""),width=30,height=20,pointsize=20);
	heatmap.2(DOX,Rowv=NA, Colv=NA, scale="none",cellnote=DOX,notecol="yellow", revC=F,labRow=CELL,labCol=vari[SIGNIFICANTS], breaks=c(seq(-1, 1, 0.1)), col =colorRampPalette(c("navy", "white", "red"))(20),margins = c(22,15));
	dev.off();

}





