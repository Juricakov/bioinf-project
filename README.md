# Projektni zadatak iz bioinformatike

Za prevođenje programa potrebmo je pokrenuti sljedeću naredbu:
g++ pafParser.cpp Graph.cpp Edge.cpp Node.cpp fasta/reader.cpp namedSequence.cpp heuristic/extensionScore.cpp pathSelector.cpp pathGenerator.cpp heuristic/overlapScore.cpp path.cpp pathMerger.cpp sequenceGenerator.cpp heuristic/monteCarlo.cpp fastq/reader.cpp main.cpp fasta/writer.cpp  

Program prima 5 argumenata kroz komandnu liniju. Prva dva argumenta su putanja do .paf datoteka sa preklapanjima (redoslijed nije bitan), druga dva argumenta su putanja do .fasta/.fastq datoteka s očitanjima (redoslijd nije bitan), a zadnje je putanja do datoteke u koju se upisuje rezultat. Tijek izvođenja programa može se pratiti zbog kontorlnih ispisa u komandnoj liniji. Primjer pokretanja programa: a overlaps1.paf overlaps2.paf ecoli_test_reads.fasta ecoli_test_contigs.fasta ecoli_full.fasta