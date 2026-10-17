function check(condition, message) {
    if (!condition)
        throw new Error(message);
}

// Qt JSON classes are bound as core value types (Qt 6.8): parse, inspect,
// serialize, and round-trip through script values.
check(typeof QJsonDocument === "function", "QJsonDocument binding was not imported");
check(typeof QJsonObject === "function", "QJsonObject binding was not imported");
check(typeof QJsonArray === "function", "QJsonArray binding was not imported");
check(typeof QJsonValue === "function", "QJsonValue binding was not imported");
check(typeof QJsonParseError === "function", "QJsonParseError binding was not imported");

var doc = QJsonDocument.fromJson(new QByteArray('{"a":1,"b":[true,"x"],"n":null}'));
check(!doc.isNull() && !doc.isEmpty() && doc.isObject(), "fromJson rejected a valid document");
var root = doc.object();
check(root.value("a").toInt() === 1, "QJsonObject value conversion failed");
check(root.value("b").toArray().at(0).toBool() === true, "nested QJsonArray bool conversion failed");
check(root.value("b").toArray().at(1).toString() === "x", "nested QJsonArray string conversion failed");
check(root.value("n").isNull(), "null QJsonValue was not reported null");
check(root.value("missing").isUndefined(), "missing key was not reported undefined");
check(root.contains("a") && root.length() === 3, "QJsonObject contains/length failed");
check(doc.toJson(QJsonDocument.Compact).toString().indexOf('"a":1') !== -1,
      "QJsonDocument Compact serialization failed");
check(doc.toJson(QJsonDocument.Indented).toString().indexOf('\n') !== -1,
      "QJsonDocument Indented serialization failed");

var empty = new QJsonDocument();
check(empty.isEmpty() && empty.isNull(), "default QJsonDocument is not empty");
var fromObj = QJsonDocument.fromVariant(root.toVariantMap());
check(!fromObj.isNull() && fromObj.object().value("a").toInt() === 1,
      "QJsonDocument.fromVariant lost object content");

var bad = QJsonDocument.fromJson(new QByteArray('{oops'));
check(bad.isNull(), "malformed JSON was not reported as a null document");

var err = new QJsonParseError();
check(err.error === QJsonParseError.NoError && err.offset === -1 && err.errorString() === "no error occurred",
      "QJsonParseError default state is wrong");

var arr = new QJsonArray();
arr.append(new QJsonValue(7));
arr.append(new QJsonValue("seven"));
check(arr.size() === 2 && arr.at(0).toInt() === 7 && arr.at(1).toString() === "seven",
      "QJsonArray append/at conversion failed");
var arrDoc = new QJsonDocument(arr);
check(arrDoc.isArray() && arrDoc.array().size() === 2, "QJsonArray document round-trip failed");

// Open, modify, and save a JSON file through QJsonObject.insert (bound with
// the discarded-return wrapper; plain bitwise flags stay numbers in script).
var filePath = "json-smoke-tmp.json";
function saveDoc(doc) {
    var f = new QFile(filePath);
    check(f.open(QIODevice.WriteOnly | QIODevice.Truncate),
          "QFile open for write failed: " + f.errorString());
    check(f.write(doc.toJson(QJsonDocument.Indented)) > 0,
          "QFile.write rejected the JSON buffer");
    f.close();
}
function loadDoc() {
    var f = new QFile(filePath);
    check(f.open(QIODevice.ReadOnly), "QFile open for read failed: " + f.errorString());
    var loaded = QJsonDocument.fromJson(f.readAll());
    f.close();
    check(!loaded.isNull(), "reloaded JSON document was reported null");
    return loaded;
}
var edit = new QJsonObject();
edit.insert("name", new QJsonValue("smoke"));
edit.insert("n", new QJsonValue(2));
saveDoc(new QJsonDocument(edit));
var reloaded = loadDoc();
check(reloaded.object().value("name").toString() === "smoke"
      && reloaded.object().value("n").toInt() === 2,
      "QFile → QJsonDocument.fromJson round-trip lost content");
var changed = reloaded.object();
changed.insert("n", new QJsonValue(changed.value("n").toInt() + 1));
changed.insert("extra", new QJsonValue(true));
reloaded.setObject(changed);
saveDoc(reloaded);
var finalDoc = loadDoc();
check(finalDoc.object().value("n").toInt() === 3 && finalDoc.object().value("extra").toBool() === true,
      "QJsonObject.insert edits did not persist through save/load");
var cleanup = new QFile(filePath);
if (cleanup.open(QIODevice.WriteOnly | QIODevice.Truncate))
    cleanup.close();
if (!new QFile(filePath).remove())
    check(false, "temp JSON file could not be removed");

print("Qt JSON smoke test passed");
