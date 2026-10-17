// QTextStream encoding validation suite (Qt 6 port).
//
// Runs in its OWN qs_eval process (see test.ps1): caught native throwError
// calls destabilize this QtScript engine for later native calls (verified
// with untouched oracle DLLs, so this is engine-level, not binding-level).
// Keeping the throw-heavy invalid-input assertions in a short-lived,
// encoding-only process keeps them deterministic. The main core regression
// suite covers everything else; this file covers setEncoding/encoding
// round trips plus rejection of invalid inputs.
function check(condition, message) {
    if (!condition) throw new Error(message);
}
function checkThrows(action, message) {
    var threw = false;
    try { action(); } catch (error) { threw = true; }
    check(threw, message);
}

// Check exact bytes as well as decoding: symmetric conversion bugs can hide
// in round trips, especially for byte order and supplementary characters.
function checkEncoding(encoding, text, hex, bom) {
    var bytes = new QBuffer();
    check(bytes.open(new QIODevice.OpenMode(QIODevice.ReadWrite)), "encoding buffer open");
    var stream = new QTextStream(bytes);
    stream.setEncoding(encoding);
    stream.setGenerateByteOrderMark(bom);
    stream.writeString(text);
    stream.flush();
    check(bytes.seek(0) && bytes.readAll().toHex().toString() === hex, "wrong encoded bytes for " + encoding);
    check(stream.status().valueOf() === QTextStream.Ok.valueOf(), "encoding write status");
    check(stream.encoding().valueOf() === encoding.valueOf(), "encoding getter lost enum type");
    check(stream.seek(0), "encoding stream seek");
    stream.setAutoDetectUnicode(bom);
    check(stream.readAll() === text, "encoding read round trip for " + encoding);
    bytes.close();
}
var unicodeText = "A\u00e9\u03a9\ud83d\ude00";
checkEncoding(QStringConverter.Utf8, unicodeText, "41c3a9cea9f09f9880", false);
checkEncoding(QStringConverter.Utf16LE, unicodeText, "4100e900a9033dd800de", false);
checkEncoding(QStringConverter.Utf16BE, unicodeText, "004100e903a9d83dde00", false);
checkEncoding(QStringConverter.Utf32LE, unicodeText, "41000000e9000000a903000000f60100", false);
checkEncoding(QStringConverter.Utf32BE, unicodeText, "00000041000000e9000003a90001f600", false);
checkEncoding(QStringConverter.Latin1, "caf\u00e9", "636166e9", false);
checkEncoding(QStringConverter.Utf8, unicodeText, "efbbbf41c3a9cea9f09f9880", true);
checkEncoding(QStringConverter.Utf16LE, unicodeText, "fffe4100e900a9033dd800de", true);

var bomBuffer = new QBuffer();
bomBuffer.setData(QByteArray.fromHex(new QByteArray("feff004100e903a9d83dde00")));
check(bomBuffer.open(new QIODevice.OpenMode(QIODevice.ReadOnly)), "BOM input buffer open");
var bomReader = new QTextStream(bomBuffer);
check(bomReader.autoDetectUnicode() && bomReader.readAll() === unicodeText,
      "UTF-16BE BOM autodetection failed with the default UTF-8 encoding");
bomBuffer.close();

var encodingProbe = new QTextStream();
var encodings = [QStringConverter.Utf8, QStringConverter.Utf16, QStringConverter.Utf16LE,
    QStringConverter.Utf16BE, QStringConverter.Utf32, QStringConverter.Utf32LE,
    QStringConverter.Utf32BE, QStringConverter.Latin1, QStringConverter.System];
for (var encodingIndex = 0; encodingIndex < encodings.length; ++encodingIndex) {
    encodingProbe.setEncoding(encodings[encodingIndex]);
    check(encodingProbe.encoding().valueOf() === encodings[encodingIndex].valueOf(), "encoding enum selection");
}
encodingProbe.setEncoding(QStringConverter.Utf8.valueOf());
var typedEncoding = new QStringConverter.Encoding(QStringConverter.Utf16LE.valueOf());
typedEncoding.valueOf = function () { return QStringConverter.Utf8.valueOf(); };
encodingProbe.setEncoding(typedEncoding);
check(encodingProbe.encoding().valueOf() === QStringConverter.Utf16LE.valueOf(),
      "setEncoding used a script valueOf override instead of the typed enum");
encodingProbe.setEncoding(QStringConverter.Utf8);
var invalidEncodings = [-1, 99, 0.5, NaN, Infinity, undefined, null, true, "UTF-8", {}, QTextStream.Ok];
for (var invalidIndex = 0; invalidIndex < invalidEncodings.length; ++invalidIndex) {
    var invalidEncoding = invalidEncodings[invalidIndex];
    checkThrows(function () { encodingProbe.setEncoding(invalidEncoding); }, "invalid encoding accepted");
    check(encodingProbe.encoding().valueOf() === QStringConverter.Utf8.valueOf(), "invalid encoding mutated stream");
}
checkThrows(function () { encodingProbe.setEncoding(); }, "missing encoding accepted");
checkThrows(function () { encodingProbe.setEncoding(0, 1); }, "extra encoding argument accepted");
checkThrows(function () { new QStringConverter.Encoding(-1); }, "invalid encoding enum accepted");
checkThrows(function () { new QStringConverter.Encoding("UTF-8"); }, "string encoding enum accepted");
print("Core encoding validation smoke passed");
