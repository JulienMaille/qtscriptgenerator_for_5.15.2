// Behavioural smoke suite for the container and value classes. The audit kept
// finding two failure modes that a presence check cannot see: a member that
// exists but does nothing when it is called, and a whole member that quietly
// stopped being generated. Each assertion below was verified against the built
// bindings before it was written.
//
// Known gaps that are measured but not asserted here, because their cause is
// still being isolated (see the reject log, generator/mjb_rejected_functions.log):
//   - QByteArray.contains: not a loss at all. The curated typesystem rejects it
//     (`<rejection class="QByteArray" function-name="contains"/>` in
//     generator/typesystem_core-qtscript.xml), and the reject log files both
//     overloads under "Generation disabled by type system" on both lines;
//   - QPolygon.push_back/prepend/insert/fill/contains/indexOf: take a dependent
//     alias (parameter_type) or QList::AT that the builder cannot resolve;
//   - QPolygon.mid() is callable but hands back a value with no members at all.
function check(condition, message) {
    if (!condition)
        throw new Error(message);
}

// A known gap is asserted so that fixing it turns this suite red: that failure
// is the reminder to delete the assertion and test the fixed behaviour instead.
// The condition is the behaviour the fix must produce, so it must be false while
// the gap is open.
function checkKnownGap(condition, gap, message) {
    if (condition)
        throw new Error(gap + " looks fixed, delete this assertion and test it: " + message);
}

// --- QPolygon / QPolygonF: the QList-derived surface that resolving the
// class-scoped type aliases (value_type) recovered. at(), takeFirst() and
// takeLast() were absent from the generated bindings before that fix.
var polygon = new QPolygon([new QPoint(1, 2), new QPoint(3, 4)]);
check(polygon.size() === 2, "QPolygon(QList<QPoint>) construction failed");
check(polygon.at(1).y() === 4, "QPolygon.at() failed");
check(polygon.value(0).x() === 1, "QPolygon.value() failed");
check(polygon.first().x() === 1 && polygon.last().y() === 4, "QPolygon.first()/last() failed");
polygon.removeAt(0);
check(polygon.size() === 1 && polygon.at(0).x() === 3, "QPolygon.removeAt() failed");
polygon.clear();
check(polygon.size() === 0 && polygon.isEmpty(), "QPolygon.clear() failed");

var resized = new QPolygon();
resized.resize(3);
check(resized.size() === 3 && resized.capacity() >= 3, "QPolygon.resize()/capacity() failed");
var swapped = new QPolygon([new QPoint(1, 1)]);
resized.swap(swapped);
check(resized.size() === 1 && swapped.size() === 3, "QPolygon.swap() failed");

var floats = new QPolygonF([new QPointF(1, 2), new QPointF(3, 4)]);
check(floats.at(0).x() === 1, "QPolygonF.at() failed");
check(floats.boundingRect().width() === 2, "QPolygonF.boundingRect() failed");
check(floats.takeLast().x() === 3 && floats.size() === 1, "QPolygonF.takeLast() failed");
check(floats.takeFirst().y() === 2 && floats.size() === 0, "QPolygonF.takeFirst() failed");

// --- Known gap (audit option C): the append() overloads take a dependent alias
// (parameter_type / const_reference) that the builder cannot resolve, so the
// call matches an overload that appends nothing -- with a point as well as with
// a QPolygon, which is the part a script author cannot see.
var appended = new QPolygon();
appended.append(new QPoint(7, 8));
checkKnownGap(appended.size() === 1, "QPolygon.append() silently drops its argument",
    "appending a point must grow the polygon");

// --- QByteArray
var bytes = new QByteArray("sample");
check(bytes.size() === 6 && bytes.length() === 6, "QByteArray size/length failed");
check(bytes.toString() === "sample", "QByteArray conversion failed");
check(bytes.at(1) === 97, "QByteArray.at() must yield the byte value");
check(new QByteArray("abc").toHex().toString() === "616263", "QByteArray.toHex() failed");
check(new QByteArray("42").toInt().valueOf() === 42, "QByteArray.toInt() failed");
check(QByteArray.fromHex(new QByteArray("6162")).toString() === "ab",
      "QByteArray.fromHex() static failed");
check(QByteArray.number(255).toString() === "255", "QByteArray.number() static failed");

var buffer = new QByteArray("ab");
buffer.append(new QByteArray("cd"));
check(buffer.toString() === "abcd", "QByteArray.append(QByteArray) failed");
buffer.prepend(new QByteArray("ab"));
check(buffer.toString() === "ababcd", "QByteArray.prepend(QByteArray) failed");
buffer.replace(new QByteArray("ab"), new QByteArray("X"));
check(buffer.toString() === "XXcd", "QByteArray.replace(QByteArray, QByteArray) failed");
check(buffer.indexOf(new QByteArray("cd")) === 2, "QByteArray.indexOf(QByteArray) failed");
check(buffer.startsWith(new QByteArray("X")) && buffer.endsWith(new QByteArray("d")),
      "QByteArray.startsWith()/endsWith() failed");
buffer.remove(0, 1);
check(buffer.toString() === "Xcd", "QByteArray.remove() failed");
buffer.chop(1);
buffer.truncate(2);
check(buffer.toString() === "Xc", "QByteArray.chop()/truncate() failed");

// --- QByteArray: the members qbytearray.h declares with a trailing ref-qualifier
// ("[[nodiscard]] QByteArray left(qsizetype) const &"). Their declarations used
// to fail in the parser - which does not read the '&' - and the recovery dropped
// them, so they never reached the bindings and only a stderr warning named them.
// On both Qt lines the lvalue-callable overload is the 'const &' one; the '&&'
// twins stay out (a wrapper holds an lvalue, so it cannot call them).
var sample = new QByteArray("sample");
check(sample.left(3).toString() === "sam", "QByteArray.left() failed");
check(sample.right(3).toString() === "ple", "QByteArray.right() failed");
check(sample.mid(1, 3).toString() === "amp", "QByteArray.mid() failed");
check(sample.first(2).toString() === "sa", "QByteArray.first() failed");
check(sample.last(2).toString() === "le", "QByteArray.last() failed");
check(sample.sliced(1, 3).toString() === "amp", "QByteArray.sliced() failed");
check(sample.chopped(1).toString() === "sampl", "QByteArray.chopped() failed");
check(sample.toUpper().toString() === "SAMPLE", "QByteArray.toUpper() failed");
check(sample.toLower().toString() === "sample", "QByteArray.toLower() failed");
check(new QByteArray("  padded  ").trimmed().toString() === "padded",
      "QByteArray.trimmed() failed");
check(new QByteArray("a  b   c").simplified().toString() === "a b c",
      "QByteArray.simplified() failed");
// The trailing NUL nullTerminated() appends does not survive the binding's
// value conversion (the returned QByteArray comes back as a 6-byte "sample"),
// so only the content is asserted here.
check(sample.nullTerminated().toString() === "sample",
      "QByteArray.nullTerminated() failed");
check(new QByteArray("a%20b").percentDecoded().toString() === "a b",
      "QByteArray.percentDecoded() failed");

// --- QDate, QTime, QDateTime
var leapDay = new QDate(2024, 2, 29);
check(leapDay.isValid() && leapDay.day() === 29 && leapDay.month() === 2 && leapDay.year() === 2024,
      "QDate accessors failed");
var monthEnd = new QDate(2024, 2, 28);
check(monthEnd.addDays(2).day() === 1, "QDate.addDays() failed");
check(monthEnd.daysTo(new QDate(2024, 3, 1)) === 2, "QDate.daysTo() failed");
var time = new QTime(1, 2, 3);
check(time.isValid() && time.hour() === 1 && time.minute() === 2 && time.second() === 3,
      "QTime accessors failed");
var stamp = new QDateTime(new QDate(2024, 1, 1), new QTime(0, 0, 0));
check(stamp.addSecs(3600).time().hour() === 1, "QDateTime.addSecs() failed");
check(stamp.toString("yyyy-MM-dd").indexOf("2024-01-01") === 0, "QDateTime.toString(format) failed");

// --- QUuid
// QUuid::createUuidV7() appears in Qt 6.9; the 6.8 LTS line declares no such
// member at all (its quuid.h has createUuid, createUuidV3 and createUuidV5),
// so this one check is versioned rather than dropped: on a line that has it,
// asking for it must work and must produce a version 7 uuid.
if (typeof QUuid.createUuidV7 === "function") {
    var generated = QUuid.createUuidV7();
    // version() hands back an enum object rather than a number: Number() reads
    // the value it carries (7), String() gives the enum key ("UnixEpoch"), and
    // === against 7 is false, which is why the comparison goes through Number().
    check(!generated.isNull() && Number(generated.version()) === 7,
          "QUuid.createUuidV7() must produce a version 7 uuid");
}
var fixedUuid = new QUuid("12345678-1234-5678-1234-567812345678");
var uuidText = fixedUuid.toString();
// QUuid::toString() keeps the braces of the canonical form.
check(uuidText.length === 38 && uuidText.charAt(0) === "{",
      "QUuid.toString() must keep the braces of the canonical form");
check(QUuid.fromString(uuidText).toString() === uuidText, "QUuid.fromString() static failed");
var fixedUuid = new QUuid("12345678-1234-5678-1234-567812345678");
check(fixedUuid.toString() === "{12345678-1234-5678-1234-567812345678}",
      "QUuid(QString) construction failed");
check(new QUuid().isNull(), "QUuid default construction failed");

// --- QPoint, QPointF, QRect, QSize, QMargins
var point = new QPoint(3, 4);
check(point.x() === 3 && point.y() === 4 && point.manhattanLength() === 7, "QPoint accessors failed");
var factor = new QPointF(1.5, 2.5);
check(factor.x() === 1.5 && factor.y() === 2.5, "QPointF accessors failed");
var rect = new QRect(1, 2, 10, 20);
check(rect.width() === 10 && rect.height() === 20 && rect.contains(new QPoint(5, 5)),
      "QRect accessors/contains failed");
var emptySize = new QSize(0, 0);
check(emptySize.isEmpty(), "QSize.isEmpty() failed");
check(emptySize.expandedTo(new QSize(2, 3)).width() === 2, "QSize.expandedTo() failed");
var margins = new QMargins(1, 2, 3, 4);
check(margins.left() === 1 && margins.top() === 2 && margins.right() === 3 && margins.bottom() === 4,
      "QMargins accessors failed");

// --- QUrl
var url = new QUrl("https://example.org/a/b?x=1");
check(url.scheme() === "https" && url.host() === "example.org" && url.path() === "/a/b",
      "QUrl accessors failed");
url.setFragment("part");
check(url.fragment() === "part", "QUrl.setFragment() failed");

// --- JSON
var document = QJsonDocument.fromJson(new QByteArray('{"a":1,"b":"two"}'));
check(document.object().value("a").toInt() === 1, "QJsonDocument.fromJson() failed");
check(document.object().value("b").toString() === "two", "QJsonObject value lookup failed");
check(document.toJson().toString().indexOf('"a"') >= 0, "QJsonDocument.toJson() failed");
