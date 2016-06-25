rm(list = ls())
library(Biobase)
library(gplots)
library(gtools)

vari=c('Differentiated spermatogonia differentiation','Preleptotene differentiation','Leptotene differentiation','Zygotene differentiation','Pachytene differentiation','Round spermatids differentiation','Differentiated spermatogonia proliferation','Diplotene proliferation','Secondary spermatocyte proliferation','Differentiated spermatogonia apoptosis_min','Preleptotene apoptosis_min','Leptotene apoptosis_min','Zygotene apoptosis_min','Pachytene apoptosis_min','Diplotene apoptosis_min','Secondary spermatocyte apoptosis_min','Round spermatid apoptosis_min','Elongated spermatid apoptosis_min','Undifferentiated spermatogonia speed','Differentiated spermatogonia speed','Preleptotene speed','Leptotene speed','Zygotene speed','Pachytene speed','Diplotene speed','Secondary spermatocyte speed','Round spermtid speed','Elongated spermatid speed','Undifferentiated spermatogonia initial number');

CELL=c('Undifferentiated spermatogonia','Differentiated spermatogonia','Preleptotene','Leptotene','Zygotene','Pachytene','Diplotene','Secondary spermatocyte','Round spermatid','Elongated spermatid','Sperm');



TIMEFRAME=round(c(seq(4.01,72.89,8.61))*24);

for (i in 1:9) {

	# PLOTS FOR PRCC VALUES
	PRCC <- read.table(paste('Z.Number_ATFMEAN/X.PRCC_',TIMEFRAME[i],'.txt',sep=""), sep="\t",header=F);

	PRCC[is.na(PRCC)]<-0
	PRCC=unname(PRCC);




	# PLOTS FOR P-VALUES
	SIGNS <- read.table(paste('Z.Number_ATFMEAN/Y.SIGNS_',TIMEFRAME[i],'.txt',sep=""), sep="\t",header=F);

	SIGNS[is.na(SIGNS)]<-1
	SIGNS=unname(SIGNS);

	pdf(file=paste('Z.Number_ATFMEAN/3.Combined_',TIMEFRAME[i],'.pdf',sep=""),width=200,height=100,pointsize=50);
	lcex = 0.5;
	par(mfrow=c(2,5),mar=c(50, 5, 4, 1));
	for (joop in 1:11) {
		P1=as.matrix(PRCC[joop,2:30]);
		
		barplot(P1,axes=F,ylim=c(-1,1),main=CELL[joop],cex.main=3.5);
		axis(2,at=c(seq(-1,1,0.2)),cex.axis=1.5,font=2,cex.lab=3);
		axis(1,at=c(seq(0.7,34.3,1.2)),labels=vari,,cex.axis=2,font=2,las=2)
		axis(1,at=c(seq(0.7,34.3,1.2)),labels=c(seq(1,29)),col.axis="cyan",cex.axis=2,font=2,las=2)
		
		POP=as.matrix(SIGNS[joop,2:30]);
		pook=c(seq(0.7,34.3,1.2));
		for (u in 1:29) {
			if (POP[u] < 0.05) {
				points(pook[u],POP[u]+0.03, pch='*', col="red", cex=8);
			}
		}
	}
	dev.off();
}














