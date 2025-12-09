#!/usr/bin/env python3
# scripts/generate_unicode_norm_tables.py
# Gera UnicodeNormalization_tables.hpp para UAX #15 (NFC/NFD/NFKC/NFKD)
# - Baixa: UnicodeData.txt, DerivedNormalizationProps.txt, NormalizationCorrections.txt, CompositionExclusions.txt
# - Parseia: decomposições (canonical + compat), CCC, corrections, composition exclusions
# - Gera arrays no formato esperado pelo seu código C++ (namespace UnicodeNormalization)
#
# Execute: python scripts/generate_unicode_norm_tables.py
# Requer Python 3.7+

import os
import sys
import urllib.request

BASE_URL = "https://www.unicode.org/Public/UCD/latest/ucd/"

FILES = {
    "UnicodeData": "UnicodeData.txt",
    "DerivedNormalizationProps": "DerivedNormalizationProps.txt",
    "NormalizationCorrections": "NormalizationCorrections.txt",
    "CompositionExclusions": "CompositionExclusions.txt"
}

OUT_FILE = "UnicodeNormalization_tables.hpp"

# ---------------- helpers ----------------
def fetch_if_missing(name):
    if os.path.exists(name):
        print(f"{name}: exists, skipping download.")
        return
    url = BASE_URL + name
    print(f"Downloading {name} from {url} ...")
    urllib.request.urlretrieve(url, name)
    print("done.")

def read_lines(path):
    with open(path, "r", encoding="utf-8") as f:
        for raw in f:
            yield raw.rstrip("\n")

# ---------------- parsing ----------------
def parse_unicode_data(path):
    """
    Parse UnicodeData.txt
    Returns:
      decomp_map: dict cp -> (compat_flag (0/1), [sequence...])  # includes canonical AND compat maps
      ccc_map: dict cp -> ccc (int)
      all_codes: set of codepoints seen
    """
    decomp_map = {}
    ccc_map = {}
    all_codes = set()

    for raw in read_lines(path):
        line = raw.split("#", 1)[0].strip()
        if not line:
            continue
        fields = line.split(";")
        if len(fields) < 6:
            continue
        try:
            cp = int(fields[0], 16)
        except:
            continue
        all_codes.add(cp)

        # field 3: canonical combining class (may be "0")
        try:
            ccc = int(fields[3])
        except:
            ccc = 0
        # always store CCC when non-zero; store zeros only if needed later (we compress ranges)
        if ccc != 0:
            ccc_map[cp] = ccc

        # field 5: Decomposition mapping (can be empty)
        decomp_field = fields[5].strip()
        if decomp_field:
            compat_flag = 0
            txt = decomp_field
            if txt.startswith("<"):
                # Tag present: <compat> / <fraction> / <super> / ...
                compat_flag = 1
                idx = txt.find(">")
                if idx != -1:
                    txt = txt[idx+1:].strip()
                else:
                    txt = ""  # malformed tag -> treat empty
            seq = [int(x, 16) for x in txt.split()] if txt else []
            # store decomposition: if same cp appears multiple times in file (shouldn't), keep last
            decomp_map[cp] = (compat_flag, seq)

    return decomp_map, ccc_map, all_codes

def parse_derived_normalization_props(path):
    """
    Parse DerivedNormalizationProps.txt for Composition_Exclusion entries.
    Returns set of codepoints.
    """
    excl = set()
    if not os.path.exists(path):
        return excl
    for raw in read_lines(path):
        line = raw.split("#", 1)[0].strip()
        if not line:
            continue
        if "Composition_Exclusion" not in line:
            continue
        left = line.split(";")[0].strip()
        if ".." in left:
            a_str, b_str = left.split("..", 1)
            a = int(a_str, 16); b = int(b_str, 16)
            for cp in range(a, b+1):
                excl.add(cp)
        else:
            excl.add(int(left, 16))
    return excl

def parse_composition_exclusions_file(path):
    """
    Parse CompositionExclusions.txt (legacy).
    Returns set of codepoints.
    """
    excl = set()
    if not os.path.exists(path):
        return excl
    for raw in read_lines(path):
        line = raw.split("#", 1)[0].strip()
        if not line:
            continue
        tok = line.split(";")[0].strip()
        if not tok:
            continue
        if ".." in tok:
            a_str, b_str = tok.split("..", 1)
            a = int(a_str, 16); b = int(b_str, 16)
            for cp in range(a, b+1):
                excl.add(cp)
        else:
            try:
                excl.add(int(tok, 16))
            except:
                pass
    return excl

def parse_normalization_corrections(path):
    """
    Parse NormalizationCorrections.txt
    Format: CP ; OLD ; NEW ; VERSION
    Return dict cp -> [new seq ints]
    """
    corr = {}
    if not os.path.exists(path):
        return corr
    for raw in read_lines(path):
        line = raw.split("#", 1)[0].strip()
        if not line:
            continue
        parts = [p.strip() for p in line.split(";")]
        if len(parts) < 3:
            continue
        try:
            cp = int(parts[0], 16)
        except:
            continue
        new_txt = parts[2].strip()
        if new_txt:
            seq = [int(x, 16) for x in new_txt.split()]
        else:
            seq = []
        corr[cp] = seq
    return corr

# ---------------- build tables ----------------
def build_decomp_tables(decomp_map):
    """
    Build sorted arrays:
     - keys[] sorted
     - offs[] sentinel appended (length keys+1)
     - targets[] concatenation of target sequences
     - flags[] compat flags aligned to keys
    """
    keys = sorted(decomp_map.keys())
    offs = []
    targets = []
    flags = []
    offset = 0
    for cp in keys:
        compat, seq = decomp_map[cp]
        offs.append(offset)
        flags.append(1 if compat else 0)
        for v in seq:
            targets.append(v)
        offset += len(seq)
    offs.append(offset)  # sentinel so consumer can do off[i+1]-off[i]
    return keys, offs, targets, flags

def build_ccc_ranges(ccc_map):
    """
    Compress ccc_map (cp->ccc) into ranges (start,end,ccc). Sorted.
    Only codepoints that have non-zero CCC are included (since CCC 0 is the default).
    """
    items = sorted(ccc_map.items(), key=lambda x: x[0])
    if not items:
        return []
    ranges = []
    start, end, val = items[0][0], items[0][0], items[0][1]
    for cp, c in items[1:]:
        if cp == end + 1 and c == val:
            end = cp
        else:
            ranges.append((start, end, val))
            start = end = cp
            val = c
    ranges.append((start, end, val))
    return ranges

def build_compose_pairs(decomp_map, ccc_map, comp_excl):
    """
    Build ALL canonical composition pairs per UAX #15:
    For each canonical decomposition C -> [A, B] we add the pair (A,B) -> C
    if:
      - mapping is canonical (compat == 0)
      - composed not in composition exclusions
      - A is a starter (CCC(A) == 0)
      - B is a combining mark (CCC(B) != 0)

    This version keeps the original 3-argument signature for compatibility
    with existing calls, but uses Python's unicodedata to determine combining class
    reliably for any code point.
    """
    import unicodedata

    pairs = {}

    for composed, (compat, seq) in decomp_map.items():
        # only canonical decompositions of length 2 are considered
        if compat == 1:
            continue
        if len(seq) != 2:
            continue
        if composed in comp_excl:
            continue

        a, b = seq

        # Use unicodedata.combining to get canonical combining class (0 => starter/base)
        # unicodedata.combining() returns 0 for non-combining, >0 for combining marks.
        # We want A to be a starter (combining == 0) and B to be a combining mark (combining != 0).
        if unicodedata.combining(chr(a)) != 0:
            continue
        if unicodedata.combining(chr(b)) == 0:
            continue

        key = (a << 32) | b
        pairs[key] = composed

    # sort pairs by key and return parallel lists
    items = sorted(pairs.items(), key=lambda kv: kv[0])
    keys = [k for k, v in items]
    results = [v for k, v in items]
    return keys, results

# ---------------- write header ----------------
def write_header(out_path,
                 ccc_ranges,
                 decomp_keys, decomp_offs, decomp_targets, decomp_flags,
                 comp_excl,
                 compose_keys, compose_results):
    with open(out_path, "w", encoding="utf-8") as f:
        f.write("// AUTO-GENERATED. DO NOT EDIT.\n")
        f.write("// Unicode Normalization tables (UAX #15) - generated by script\n\n")
        f.write("#pragma once\n\n")
        f.write("#include <cstdint>\n\n")
        f.write("namespace UnicodeNormalization {\n\n")

        # CCC ranges
        f.write(f"static constexpr uint32_t CCC_RANGES_COUNT = {len(ccc_ranges)}u;\n")
        f.write("static constexpr CodePoint CCC_RANGES[] = {\n")
        for s, e, v in ccc_ranges:
            f.write(f"  0x{s:04X}, 0x{e:04X}, {v},\n")
        f.write("};\n\n")

        # DECOMP arrays
        f.write(f"static constexpr uint32_t DECOMP_COUNT = {len(decomp_keys)}u;\n")
        f.write("static constexpr uint32_t DECOMP_KEYS[] = {\n")
        for cp in decomp_keys:
            f.write(f"  0x{cp:04X},\n")
        f.write("};\n\n")

        f.write("static constexpr uint32_t DECOMP_OFFS[] = {\n")
        for o in decomp_offs:
            f.write(f"  {o},\n")
        f.write("};\n\n")

        f.write("static constexpr uint32_t DECOMP_TARGETS[] = {\n")
        for t in decomp_targets:
            f.write(f"  0x{t:04X},\n")
        f.write("};\n\n")

        f.write("static constexpr uint8_t DECOMP_FLAGS[] = {\n")
        for fl in decomp_flags:
            f.write(f"  {fl},\n")
        f.write("};\n\n")

        # Composition exclusions
        f.write(f"static constexpr uint32_t COMP_EXCL_COUNT = {len(comp_excl)}u;\n")
        f.write("static constexpr uint32_t COMP_EXCL[] = {\n")
        for cp in sorted(comp_excl):
            f.write(f"  0x{cp:04X},\n")
        f.write("};\n\n")

        # Compose pairs
        f.write(f"static constexpr uint32_t COMPOSE_COUNT = {len(compose_keys)}u;\n")
        f.write("static constexpr uint64_t COMPOSE_PAIRS[] = {\n")
        for k in compose_keys:
            f.write(f"  0x{k:016X}ULL,\n")
        f.write("};\n\n")

        f.write("static constexpr uint32_t COMPOSE_RESULT[] = {\n")
        for v in compose_results:
            f.write(f"  0x{v:04X}u,\n")
        f.write("};\n\n")

        # alias for older consumers
        f.write("static constexpr uint32_t COMPOSE_RESULTS[] = {\n")
        for v in compose_results:
            f.write(f"  0x{v:04X}u,\n")
        f.write("};\n\n")

        f.write("} // namespace UnicodeNormalization\n")

    print("Wrote", out_path)

# ---------------- verification helpers ----------------
def check_presence(decomp_keys, decomp_flags, decomp_targets, target_cp):
    """
    Return (found_key, is_compat, targets_list) if key found, else None
    """
    try:
        idx = decomp_keys.index(target_cp)
    except ValueError:
        return None
    flag = decomp_flags[idx]
    off = decomp_offs[idx]
    next_off = decomp_offs[idx+1]
    seq = decomp_targets[off:next_off]
    return (True, flag, seq)

# ---------------- main ----------------
def main():
    print("Starting generation of", OUT_FILE)
    # fetch files
    for name in FILES.values():
        try:
            fetch_if_missing(name)
        except Exception as e:
            print(f"Failed to fetch {name}: {e}", file=sys.stderr)
            sys.exit(1)

    # parse UnicodeData
    print("Parsing UnicodeData...")
    decomp_map, ccc_map, all_codes = parse_unicode_data(FILES["UnicodeData"])
    print(f"  decompositions parsed: {len(decomp_map)} ; ccc entries: {len(ccc_map)} ; total codes: {len(all_codes)}")

    # parse composition exclusions
    print("Parsing DerivedNormalizationProps (Composition_Exclusion)...")
    comp_excl_dnp = parse_derived_normalization_props(FILES["DerivedNormalizationProps"])
    print(f"  DNP composition exclusions: {len(comp_excl_dnp)}")

    print("Parsing CompositionExclusions.txt (legacy)...")
    comp_excl_file = parse_composition_exclusions_file(FILES["CompositionExclusions"])
    print(f"  CompositionExclusions.txt entries: {len(comp_excl_file)}")

    comp_excl = comp_excl_dnp | comp_excl_file
    print(f"  total composition exclusions: {len(comp_excl)}")

    # parse normalization corrections and apply
    print("Parsing NormalizationCorrections...")
    corrections = parse_normalization_corrections(FILES["NormalizationCorrections"])
    print(f"  corrections entries: {len(corrections)}")
    applied = 0
    for cp, seq in corrections.items():
        if cp in decomp_map:
            compat, _ = decomp_map[cp]
            decomp_map[cp] = (compat, seq)
            applied += 1
    print(f"  applied {applied} corrections")

    # build tables
    print("Building decomposition tables...")
    decomp_keys, decomp_offs, decomp_targets, decomp_flags = build_decomp_tables(decomp_map)
    print(f"  DECOMP_COUNT = {len(decomp_keys)} ; total targets = {len(decomp_targets)}")

    print("Building CCC ranges...")
    ccc_ranges = build_ccc_ranges(ccc_map)
    print(f"  CCC_RANGES_COUNT = {len(ccc_ranges)}")

    print("Building composition pairs...")
    compose_keys, compose_results = build_compose_pairs(decomp_map, ccc_map, comp_excl)
    print(f"  COMPOSE_COUNT = {len(compose_keys)}")

    # write header
    write_header(OUT_FILE,
                 ccc_ranges,
                 decomp_keys, decomp_offs, decomp_targets, decomp_flags,
                 comp_excl,
                 compose_keys, compose_results)

    # quick verification of previously failing cases
    print("\nVerification checks (quick):")
    def has_decomp(cp_hex):
        cp = int(cp_hex, 16)
        try:
            idx = decomp_keys.index(cp)
            flag = decomp_flags[idx]
            off = decomp_offs[idx]
            nxt = decomp_offs[idx+1]
            seq = decomp_targets[off:nxt]
            return True, bool(flag), seq
        except ValueError:
            return False, None, None

    checks = [
        ("00BC", "VULGAR FRACTION ONE QUARTER"),
        ("00BD", "VULGAR FRACTION ONE HALF"),
        ("00BE", "VULGAR FRACTION THREE QUARTERS"),
        ("01C6", "LATIN SMALL LETTER DZ WITH CARON"),
        ("01C4", "LATIN CAPITAL LETTER DZ WITH CARON"),
        ("01C5", "LATIN CAPITAL LETTER D WITH SMALL LETTER Z WITH CARON"),
    ]
    checks.extend([
        ("017E", "LATIN SMALL LETTER Z WITH CARON"),
        ("017D", "LATIN CAPITAL LETTER Z WITH CARON"),
    ])
    for hx, desc in checks:
        ok, iscompat, seq = has_decomp(hx)
        if ok:
            seq_str = " ".join(f"U+{x:04X}" for x in seq) if seq else "(empty)"
            print(f"  {hx} ({desc}): present; compat={iscompat}; seq={seq_str}")
        else:
            print(f"  {hx} ({desc}): MISSING in DECOMP_KEYS!")

    # check CCC 0303..0305
    for cp_hex in ("0303","0304","0305"):
        cp = int(cp_hex, 16)
        in_ccc = any(s <= cp <= e for s,e,_ in ccc_ranges)
        print(f"  U+{cp_hex}: CCC range present? {in_ccc}")

    print("\nGeneration finished. If any 'MISSING' appears above, re-run and verify the input data files are the official UnicodeData.txt / DerivedNormalizationProps.txt / NormalizationCorrections.txt files (no truncation).")

if __name__ == "__main__":
    main()