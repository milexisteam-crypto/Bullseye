const fs = require("fs");
const path = require("path");
const { execSync } = require("child_process");

// =========================
// CONFIG
// =========================

const OUTPUT = "Bullseye";

let TOOLCHAIN = "";
let TOOLCHAIN_PREFIX = TOOLCHAIN ? TOOLCHAIN + "-" : "";

const CC  = TOOLCHAIN_PREFIX + "gcc";
const CXX = TOOLCHAIN_PREFIX + "g++";
const LD  = TOOLCHAIN_PREFIX + "ld"; // 👈 IMPORTANT: BFD LINKER (stable)
const AS  = "nasm";

// =========================
// FLAGS (FIXED)
// =========================

let CXXFLAGS = [
    "-g",
    "-O2",
    "-pipe",
    "-Wall",
    "-Wextra",
    "-ffreestanding",

    "-fno-stack-protector",
    "-fno-stack-check",

    "-fno-pie",      // 🔥 FIX
    "-fno-pic",      // 🔥 FIX

    "-fno-lto",

    "-ffunction-sections",
    "-fdata-sections",

    "-m64",
    "-march=x86-64",
    "-mabi=sysv",
    "-mno-red-zone",
    "-mcmodel=kernel",

    "-fno-exceptions",
    "-fno-rtti"
].join(" ");

let CPPFLAGS = "-I src -I src/include -MMD -MP";

let NASMFLAGS = "-f elf64 -g -F dwarf";

// =========================
// LINKER FLAGS (CRITICAL FIX)
// =========================

let LDFLAGS = [
    "-nostdlib",
    "-static",
    "-T linker.lds",
    "-z max-page-size=0x1000",
    "--gc-sections",

    // 🔥 CRITICAL FIXES FOR LIMINE
    "-no-pie",
    "-z noexecstack"
].join(" ");

// =========================
// FILE DISCOVERY
// =========================

function findFiles(dir) {
    let results = [];
    for (const file of fs.readdirSync(dir)) {
        const full = path.join(dir, file);
        const stat = fs.statSync(full);
        if (stat.isDirectory()) {
            results = results.concat(findFiles(full));
        } else {
            results.push(full);
        }
    }
    return results;
}

const SRCFILES = findFiles("src");

const CPPFILES = SRCFILES.filter(f => f.endsWith(".cpp"));
const CFILES   = SRCFILES.filter(f => f.endsWith(".c"));
const SFILES   = SRCFILES.filter(f => f.endsWith(".S"));
const ASMFILES = SRCFILES.filter(f => f.endsWith(".asm"));

function objPath(src) {
    return path.join("obj", src.replace(/^src\//, "") + ".o");
}

const OBJECTS = [
    ...CPPFILES.map(objPath),
    ...CFILES.map(objPath),
    ...SFILES.map(objPath),
    ...ASMFILES.map(objPath)
];

// =========================
// HELPERS
// =========================

function run(cmd) {
    console.log(">>", cmd);
    execSync(cmd, { stdio: "inherit" });
}

function ensureDir(file) {
    const dir = path.dirname(file);
    if (!fs.existsSync(dir)) fs.mkdirSync(dir, { recursive: true });
}

// =========================
// COMPILE
// =========================

function compile() {
    for (const file of CPPFILES) {
        const out = objPath(file);
        ensureDir(out);
        run(`${CXX} ${CXXFLAGS} ${CPPFLAGS} -c ${file} -o ${out}`);
    }

    for (const file of CFILES) {
        const out = objPath(file);
        ensureDir(out);
        run(`${CC} ${CXXFLAGS} ${CPPFLAGS} -c ${file} -o ${out}`);
    }

    for (const file of SFILES) {
        const out = objPath(file);
        ensureDir(out);
        run(`${CC} ${CXXFLAGS} ${CPPFLAGS} -c ${file} -o ${out}`);
    }

    for (const file of ASMFILES) {
        const out = objPath(file);
        ensureDir(out);
        run(`${AS} ${NASMFLAGS} ${file} -o ${out}`);
    }
}

// =========================
// LINK
// =========================

function link() {
    if (!fs.existsSync("bin")) fs.mkdirSync("bin");

    const objList = OBJECTS.join(" ");

    // 🔥 IMPORTANT: BFD ld (NOT lld)
    run(`${LD} ${LDFLAGS} ${objList} -o bin/${OUTPUT}`);
}

// =========================
// ISO
// =========================

function generateISO() {
    console.log("=== ISO ===");

    run("rm -rf obj/iso_root");
    run("mkdir -p obj/iso_root/boot");
    run("mkdir -p obj/iso_root/boot/limine");
    run("mkdir -p obj/iso_root/EFI/BOOT");
run("mkdir -p obj/iso_root/Resources");
    run("cp -v bin/Bullseye obj/iso_root/boot/");
    run("cp -v Resources/* obj/iso_root/Resources/");
    run("cp -v limine.conf obj/iso_root/boot/limine/");
    run("cp -v limine/limine-bios.sys obj/iso_root/boot/limine/");
    run("cp -v limine/limine-bios-cd.bin obj/iso_root/boot/limine/");
    run("cp -v limine/limine-uefi-cd.bin obj/iso_root/boot/limine/");

    run("cp -v limine/BOOTX64.EFI obj/iso_root/EFI/BOOT/");
    run("cp -v limine/BOOTIA32.EFI obj/iso_root/EFI/BOOT/");

    run(
        "xorriso -as mkisofs -R -r -J " +
        "-b boot/limine/limine-bios-cd.bin " +
        "-no-emul-boot -boot-load-size 4 -boot-info-table " +
        "-hfsplus -apm-block-size 2048 " +
        "--efi-boot boot/limine/limine-uefi-cd.bin " +
        "-efi-boot-part --efi-boot-image --protective-msdos-label " +
        "obj/iso_root -o bin/Bullseye.iso"
    );
}

// =========================
// RUN
// =========================

function runOs() {
    run("qemu-system-x86_64 -cdrom bin/Bullseye.iso -no-reboot -d cpu_reset");
}

// =========================
// CLEAN
// =========================

function clean() {
    if (fs.existsSync("obj")) fs.rmSync("obj", { recursive: true });
    if (fs.existsSync("bin")) fs.rmSync("bin", { recursive: true });
}

// =========================
// MAIN
// =========================

const cmd = process.argv[2] || "all";

switch (cmd) {
    case "all":
        clean();
        compile();
        link();
        generateISO();
        break;

    case "run":
        clean();
        compile();
        link();
        generateISO();
        runOs();
        break;

    case "clean":
        clean();
        break;
    case "check":
            clean()
            compile()
    default:
        console.log("Unknown command:", cmd);
}