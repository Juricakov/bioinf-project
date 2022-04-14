const fs = require("fs");

// always generates 2 contigs
// node generate.js {sequence_length} {reading_count} {reading complement probability}

function randomIndexInRange(min, max) {
  return min + Math.floor(Math.random() * (max - min));
}

const bases = ["C", "T", "G", "A"];
const sequenceLength = process.argv[2];

const originalSequence = Array.from({ length: sequenceLength }, () => {
  return randomIndexInRange(0, bases.length);
})
  .map((index) => bases[index])
  .join("");

const contig1StartIndex = 0;
// first contig is the first 30%-40% of the sequence
const contig1EndIndex = randomIndexInRange(
  Math.floor(originalSequence.length * 0.3),
  Math.floor(originalSequence.length * 0.4)
);

const contig1 = originalSequence.substring(contig1StartIndex, contig1EndIndex);

// second contig is the last 30%-40% of the sequence
const contig2StartIndex = randomIndexInRange(
  Math.floor(originalSequence.length * 0.6),
  Math.floor(originalSequence.length * 0.7)
);

const contig2EndIndex = originalSequence.length;

const contig2 = originalSequence.substring(contig2StartIndex, contig2EndIndex);

// bridge sequence is a sequence that overlaps with both contigs by 4 bases
const bridgeSequence = originalSequence.substring(
  contig1EndIndex - 4,
  contig2StartIndex + 4
);

const readingCount = process.argv[3];
const singleReadingSize = Math.floor(bridgeSequence.length / readingCount);
const readingComplementProbability = process.argv[4];

const baseToComplement = {
  C: "G",
  T: "A",
  G: "C",
  A: "T",
};

function complement(sequence) {
  return [...sequence]
    .map((base) => baseToComplement[base])
    .reverse()
    .join("");
}

const readings = [];

for (let i = 0; i < readingCount; i++) {
  // reading overlap of 4 bases
  readingStart = Math.max(0, i * singleReadingSize - 2);

  readingEnd =
    i == readingCount - 1
      ? bridgeSequence.length
      : (i + 1) * singleReadingSize + 2;

  let reading = bridgeSequence.substring(readingStart, readingEnd);
  if (Math.random() < readingComplementProbability) {
    reading = complement(reading);
  }

  readings.push(reading);
}

function writeFASTA(sequences, namePrefix, fileName) {
  const lines = [];
  for (let i = 0; i < sequences.length; i++) {
    lines.push(`>${namePrefix}${sequences.length === 1 ? "" : i}`);
    lines.push(sequences[i]);
  }

  fs.writeFileSync(fileName, lines.join("\n"));
}

writeFASTA([originalSequence], "original", "original.fasta");
writeFASTA([contig1, contig2], "ctg", "contigs.fasta");
writeFASTA(readings, "read", "readings.fasta");
