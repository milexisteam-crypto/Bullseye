#!/usr/bin/env node
const fs = require("fs");

// ===============================
// ARGUMENTY
// ===============================

const inFile = process.argv[2];
const outFile = process.argv[3];

if (!inFile || !outFile) {
    console.log("Usage: node bdf2cpp.js <input.bdf> <output.hpp>");
    process.exit(1);
}

// ===============================
// WCZYTANIE PLIKU
// ===============================

if (!fs.existsSync(inFile)) {
    console.error("Input file not found:", inFile);
    process.exit(1);
}

const data = fs.readFileSync(inFile);

// ===============================
// GENEROWANIE C++ HEADERA
// ===============================

let out = "";
out += "#pragma once\n";
out += "#include <stdint.h>\n\n";
out += "// Auto-generated BDF font blob\n\n";
out += "static const uint8_t font_bdf[] = {\n    ";

for (let i = 0; i < data.length; i++) {
    const hex = "0x" + data[i].toString(16).padStart(2, "0");
    out += hex + ",";

    // formatowanie (ładne linie)
    if ((i + 1) % 16 === 0) {
        out += "\n    ";
    } else {
        out += " ";
    }
}

out += "\n};\n\n";
out += "static const uint64_t font_bdf_size = sizeof(font_bdf);\n";

// ===============================
// ZAPIS
// ===============================

fs.writeFileSync(outFile, out);

console.log("✔ Converted:", inFile, "→", outFile);
console.log("Size:", data.length, "bytes");