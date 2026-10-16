"""Merge typesystem triples via lxml (replaces generate.sh xsltproc step)."""
import datetime
import pathlib

from lxml import etree

ROOT = pathlib.Path(__file__).resolve().parent
OUTDIR = ROOT / "merged"
MODULES = [
    "core", "gui", "widgets", "multimedia", "printsupport", "svg",
    "network", "opengl", "xml", "sql", "xmlpatterns",
]

OUTDIR.mkdir(parents=True, exist_ok=True)
xsl_doc = etree.parse(str(ROOT / "merge.xsl"))
transform = etree.XSLT(xsl_doc)

ok_count = 0
for mod in MODULES:
    common = ROOT / f"typesystem_{mod}-common.xml"
    qtscript = ROOT / f"typesystem_{mod}-qtscript.xml"
    out = OUTDIR / f"typesystem_{mod}.xml"
    if not common.is_file() or not qtscript.is_file():
        print(f"{mod}: SKIP")
        continue
    src_uri = qtscript.resolve().as_uri()
    result = transform(etree.parse(str(common)),
                       source=etree.XSLT.strparam(src_uri))
    out.write_bytes(etree.tostring(result, pretty_print=True,
                                   xml_declaration=True, encoding="UTF-8"))
    print(f"{mod}: OK merged={out.stat().st_size}B")
    ok_count += 1

print(f"done: {ok_count}/{len(MODULES)} merged")
