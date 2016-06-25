#!/usr/local/bin/perl -w
# Debjit Ray
# Date 07/10/2013
# This code matches the LHS parameters in the my.txt file and creates a new parameter file for each run combination
# Number of New_ .txt files in the TEST_Parameter sdepends on the number of runs
use strict;

my $inputfile  = "my.txt";
open(FDR,"<$inputfile") or die "Can't open $inputfile: $!\n";

my %hash=();
# CREATING THE HASH FOR THE DESIRED PARAMETER CHANGES
foreach(<FDR>) {
  (my $line = $_) =~ s/\r*\n//;
  next if ($line =~ /^\s/);
  my ($name, @para)=split(/\t/,$line);
  my $kool = join("\t",@para);
  my (@rook)=split(/\_/,$name);;
  if ($rook[scalar(@rook)-1] !~ /MIN/ ) {
    # MATCHING THE NUMBER
    if ($rook[scalar(@rook)-1] =~ /NUM/) {
      $hash{$name}=$kool;
      $hash{$name}=$kool;
    }
    # OTHERS WOULD HAVE MAX AND MIN BOTH REPLACED
    else {
      my $Max="_MAX";
      my $Min="_MIN";
      $hash{($name.$Max)}=$kool;
      $hash{($name.$Min)}=$kool;
    }
  }
  # ONLY THE MIN FOR APOPTOSIS IS CHANGED
  else {
    $hash{$name}=$kool;
    $hash{$name}=$kool;
  }   
}

my $runs=300;   # UPDATE this based on the number of runs

# THIS IS THE LOOP DEPENDING ON THE RUNS
for (my $i=0; $i <$runs ; $i++) {
  
  # CREATING A NEW PARAMETER FILE FOR EACH COMBINATION
  my $outputfile = "TEST_Parameter/New_$i.txt";
  open(FDW,">$outputfile") or die "Can't open $outputfile: $!\n";
 
  # READING THE ORIGINAL PARAMETER FILE
  my $inputfile2  = "parameters_1.txt";
  open(FDR2,"<$inputfile2") or die "Can't open $inputfile2: $!\n";
  
  foreach(<FDR2>) {
    (my $line = $_) =~ s/\r*\n//;
    next if ($line =~ /^\s/);
    if ($line =~ /^\/\//) {
      print FDW $line."\n";
      next;
    }
    my ($name, @para)=split(/\b/,$line);
    if (exists $hash{$name}) {
      my @tool = split(/\t/,$hash{$name});
      print FDW $name." ".$tool[$i]."\n";
    }
    else {
      print FDW $line."\n";
    }
  }
  print $i."\n";
  close(FDR2);
}

close(FDR);






