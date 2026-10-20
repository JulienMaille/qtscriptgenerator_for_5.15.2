# Audit du générateur Qt 6.12 (fork `qt6`) — pertes prouvées, causes racines, propositions

**Objet** — Finir l'audit du générateur Qt6 porté contre les projets bindings de référence : établir
la liste *exacte* des pertes d'API, isoler la cause racine de chacune, proposer un correctif.
**État** — Les correctifs F1 (`d0b1a62`), F5 option B (`84eb2d7`), P-D1 (`2019394`), F2 (`a54e49b`),
F7 (`6bc3c1a`), F5.1 (`b5c9dee`) et F8 (`423dfac`) sont livrés dans le fork sur `qt6` ; le dépôt
bindings n'a reçu aucune modification. Paire CI verte sur `ca0ec36` (6.12 `39415102699`,
6.8 LTS `39415102876`).

---

## 0. En une page

1. **Le port n'est pas une référence Qt 6.** Son `generated_cpp` est l'arbre de bindings **Qt 5.15.2**
   historique du projet, élagué à la main vers Qt 6 (`QSignalMapper`, `QFile::readLink`,
   `QDateTime::utcOffset`, `QFont::rawMode`, `QTextCharFormat::additionalFormats`,
   `QLibraryInfo::LibraryLocation`, `QList::setSharable`, `fromList`…). Sa CI **ne régénère rien** :
   elle compile et teste l'instantané versionné. Le port ne sert donc plus que de **source de curation**.
2. Conséquence : sur les **416 entrées « port-only »**, **410 sont aussi absentes de l'artefact 6.8 du
   fork** — elles ne mesurent ni une dérive Qt 6.9+ ni une perte du générateur, mais l'API Qt 5 que le
   port n'a pas encore élaguée. La référence valable est : **en-têtes Qt 6.12 + artefact 6.8 produit par
   la CI du fork lui-même**.
3. Ce qui reste est **court** : **5 familles de pertes réelles** (F1–F5, chacune avec cause racine isolée
   par reproduction minimale), **1 famille de rejets journalisés** (F6) à trier et **2 défauts de
   reproductibilité** (F7, F8) qui touchent les *valeurs d'enum*.
   **État au 21/10/2026 :** F1 corrigé (`d0b1a62`), F2 corrigé (`a54e49b`, surface 8 852 → 8 861),
   F5 livré en option B (`84eb2d7`) puis P-D1 (`2019394`) puis F5.1 (`b5c9dee`), F7 corrigé
   (`6bc3c1a`, arbre reproductible, `QSizePolicy` à ses 7 valeurs), F8 corrigé (`423dfac`,
   `QLocalSocket::LocalSocketError` à ses 11 valeurs sur 6 regens non seedés). Les runs CI 6.12
   (`39415102699`) et 6.8 LTS (`39415102876`) sur `ca0ec36` sont **verts** tous les deux :
   `QPolygon::fill` est joignable sur 6.8.3, comme prévu par F5.1. F3/F4 ouverts, F6 à trier.
4. **4 faux positifs** sont retirés de la liste (dont `QDateTime::startOfDay`).
5. Trois familles sont des **régressions du fork** par rapport à la base `689f13e` : la référence
   (`QByteArray::left/mid`, `QUuid::createUuid` statique, `QPolygon::at/indexOf`) les traitait
   correctement.
6. **Aucune de ces pertes n'apparaissait dans un journal de rejet** — c'était le vrai défaut. `exit=0` et
   un log de rejets vert ne prouvent rien. F7 le confirme autrement : deux regens identiques pouvaient
   produire des arbres **différents**, dont l'un avec des **valeurs d'enum fausses**, sans qu'aucun
   journal ni aucun test ne bronche. F1/F2/F7/F8 sont maintenant instrumentés ; le budget de rejets **par
   module** (mécanisme B) est livré (`a23b479`) et la gate est **branchée dans les deux workflows**
   (les effectifs 6.8.3 de l'artefact du run vert `39415102876` sont tous inférieurs à ceux semés sur
   6.12 : un fichier couvre les deux lignes). Le **contrat de surface versionné** (mécanisme A) est
   livré et branché lui aussi : `tools/surface-contract.ps1` + **un contrat par ligne Qt**
   (`generator/surface-contract.json` 6.12 : 457 classes / 8 861 membres ;
   `generator/surface-contract-qt683.json` 6.8.3 : 456 / 8 790), check après la génération — voir §5.A.
   Les **valeurs** et la **construction** sont vérifiées de la même façon, par des suites engendrées
   depuis l'arbre généré : les **4 823 constantes** des 473 enums sont comparées à un oracle compilé
   contre les en-têtes Qt, et les 457 classes sont enregistrées, dotées d'un prototype, puis construites
   pour les 381 qui ont un constructeur sans argument — voir §5.C.

---

## 1. Méthode et artefacts

| Artefact | Contenu | Rôle |
| --- | --- | --- |
| `D:\Codez\qtscriptgenerator-qt6-bindings\generated_cpp` | arbre du port | **curation seulement** (arbre Qt 5.15.2 élagué) |
| `…\qtscript_probe\ci-683\generated_cpp` | artefact CI 6.8.3 du fork (`windows-lts` run `39118816159`) | second point de mesure : sépare dérive Qt et comportement du générateur |
| `D:\Codez\qtscriptgenerator_for_5.15.2\generated_cpp` | régénération locale 6.12 du fork | surface cible |
| `…\qtscript_probe\pp-core-backup.tmp` | flux préprocessé **consommé** par le générateur (7 Mo) | preuve directe de ce que voit le parseur |
| `…\qtscript_probe\ast-min.xml` | dump du DOM du parseur (`--ast-to-xml`) | isole la couche fautive (parseur / builder / typesystem) |
| `…\qtscript_probe\micro*.h`, `micro*-out` | micro-sondes de déclaration | reproductions minimales des causes racines |
| `…\qtscript_probe\audit-all\<module>\mjb_rejected_*.log` | 9 367 → 13 967 rejets (9 modules) | rejets **journalisés** |

Mesures de surface — clés uniques `(module, classe, type, nom)` (extraction `surface-extract.ps1`) :

```
port (arbre Qt 5.15.2)             : 11 232
fork 6.8.3 (artefact CI)           : 14 602
fork 6.12.0 (regen locale)         : 14 691

port-only vs 6.12 : 416  … dont présentes dans le 6.8 du fork : 6
                            (410 absentes des deux → contenu du port, pas perte)
dérive fork 6.8 → fork 6.12 : 14 entrées retirées, 103 ajoutées
```

*Limite connue de l'instrument* : une clé `(module, classe, type, nom)` écrase les surcharges de même
nom (ex. `QUuid::createUuidV3` statique **et** en instance). Le diff exact par **signature** reste à
faire (partie du chantier A).

---

## 2. Le cadrage corrigé : le port n'est pas une référence Qt 6

### 2.1 Preuve par le contenu
API présente dans l'arbre du port et **impossible** en Qt 6 : `QSignalMapper` (classe supprimée en
Qt 6.0), `QFile::readLink`, `QDateTime::utcOffset/setUtcOffset`, `QFont::rawMode`,
`QTextCharFormat::additionalFormats`, `QList::setSharable`, `QLibraryInfo::LibraryLocation`
(jeu Qt 5 : `ImportsPath`, pas de `QmlImportsPath`). Classes Qt 5 absentes de son arbre car élaguées :
`QMatrix`, `QDesktopWidget`, `QTextCodec`, `QLinkedList`, `QStringRef`, `QGLWidget`.

Le port **déclare** ces symboles Qt 5 dans son propres typesystem fusionné
(`.work/merged-ts/typesystem_core.xml:1634` `<object-type name="QSignalMapper">`) alors qu'il
*retire* `readLink` (`typesystem_core.xml:1104 remove="all"`) — donc son arbre n'est pas la sortie de
ce typesystem : c'est un **instantané importé**.

### 2.2 Preuve par la CI du port
`windows-lts.yml` / `windows-latest.yml`, étapes : *Check out bindings → Check out QtScript port →
Install Qt → Build QtScript dependency → **Build bindings** → Run smoke tests → Upload*. Aucune étape
de génération, aucun téléversement d'arbre généré. Le `generated_cpp` versionné est donc maintenu à la
main, et « le port fonctionne en 6.12 » ne démontre **rien** sur le générateur.

### 2.3 La dérive 6.8 → 6.12 du fork est petite et lisible
14 retraits (dont `QReadWriteLock.lockForRead/lockForWrite/unlock`, `QGradient.stops`,
`QHostAddress.parseSubnet`, `QByteArray.percentDecoded`, 6 clés d'enum `QSizePolicy` renommées
`Policy.*`) et 103 ajouts (API 6.9–6.12 réellement captée : rôles `Qt::ItemDataRole`, `QStyle::ComboBox`,
`QLayout::setVerticalSizeConstraint`, `QXmlStreamWriter::stopWritingOnError`, `QPainterPath::trimmed`…).
La paire d'artefacts CI 6.8 / 6.12 constitue donc un excellent **détecteur de régression** (A).

---

## 3. Pertes réelles, avec cause racine isolée

### F1 — Fonctions membres **ref-qualifiées** (`const &` / `&&`) : corrigé le 19/10/2026 (`d0b1a62`)

| | |
| --- | --- |
| Perdu (mesuré) | 17 membres dans les classes à wrapper, sur 6.12 : `QByteArray` 13 (`left`, `right`, `mid`, `first`, `last`, `sliced`, `chopped`, `trimmed`, `simplified`, `toLower`, `toUpper`, `percentDecoded`, `nullTerminated`) et `QImage` 4 (`convertedTo`, `mirrored`, `rgbSwapped`, `flipped`) |
| Preuve flux | `[[nodiscard]] QByteArray mid(qsizetype, qsizetype = -1) const &;` **et** `&&` ; témoin conservé : `QByteArray leftJustified(qsizetype, char = ' ', bool = false) const;` (non qualifiée) → lié |
| Preuve DOM | `--ast-to-xml` : `<function name="QByteArray::left">` / `mid` **absentes du DOM**, `leftJustified` présente → perte **au parseur**, pas au builder |
| Preuve runtime | Avant : `QByteArray.prototype.left/mid/toLower/trimmed` = `undefined`. Après : 13 assertions de comportement dans `tests/collections-smoke.js` passent sur 6.12 |
| Cause racine | Le parseur hérité (grammaire C++03) ne connaît pas les *ref-qualifiers* de fonction : la déclaration désynchronise et la fonction disparaît avant le *binder* |
| Régressions | **Oui** : l'arbre de référence (base `689f13e`, Qt 5.15 dont les déclarations étaient simples) lie `left/mid/right/cbegin…` |
| Journalisé avant | Rien dans le journal de rejets. Seul un `WARNING … reportDroppedDeclaration` sur stderr nommait la déclaration, sans compter le membre |
| Correctif livré | `Parser::parseRefQualifier()` consomme `&` / `&&` en fin de déclarateur (les deux chemins), le déclarateur retient la forme rvalue, `isSimilar()` distingue les deux surcharges. Le générateur émet donc les formes appelables sur une lvalue (aucun qualifieur, `&`, `const &`) |
| Journalisé après | Deux sections nommées : `Only callable on an rvalue (ref-qualified '&&')` et `Return type is deduced ('auto'), not nameable` (voir F1.2) |
| Mesure | Surface 6.12 : **8 805 → 8 822**, +17, **0 perdu**. Surface 6.8.3 (artefact CI de la même révision vs l'artefact précédent) : **8 743 → 8 755**, +15 / −3, les trois « pertes » étant la reclasification `prototype → static` de `QUuid::createUuid/V3/V5` livrée par `523015b` (le quatrième cas, `QQuaternion::fromEulerAngles`, n'apparaît pas sur cette ligne). Les gains 6.8.3 sont 11 membres de `QByteArray` + 3 de `QImage` : là, les formes sans ref-qualifier de `qbytearray.h` sont sous `QT_CORE_REMOVED_SINCE(6, 7)`, donc inactives, et ce sont les `const &` qui portent l'API. `nullTerminated` **n'existe pas** dans l'en-tête 6.8.3, d'où une assertion versionnée comme `QUuid::createUuidV7`. `QImage::colorTransformed` / `convertedToColorSpace` restent dehors pour une autre cause (F1.3) |

> À ne pas confondre avec le filtre volontaire du fork sur les **paramètres** `T&&`
> (`binder.cpp`, `hasRvalueReference`, avertissement `** WARNING skipping rvalue-reference function:`).
> Ce filtre est légitime (commentaire exact) : une fonction prenant `T&&` n'est pas appelable depuis un
> binding. En revanche, une fonction **ref-qualifiée** *est* la surcharge lvalue : l'écarter perd l'API.

#### F1.1 — Ce que le correctif refuse, et pourquoi c'est nommé

Deux ensembles distincts, tous deux désormais comptés dans `generator/mjb_rejected_functions.log`.

| | |
| --- | --- |
| `&&`-seul | `QBitArray::inverted_inplace() &&` : aucun jumeau appelable sur une lvalue, donc aucun script ne peut l'atteindre. 23 déclarations refusées sur core+gui, dont les **jumeaux** `&&` de membres dont la variante `&` est émise (`QByteArray`, `QImage::convertToFormat`, `QList::operator+`…) |
| Pourquoi rien n'est perdu | Le wrapper tient un pointeur, une lvalue : appeler un `&&`-qualifié depuis lui ne compile pas, et un objet script est lui aussi une lvalue |
| Limite de comptage | La clé du journal est `Classe::signature` : le jumeau `&&` de `QImage::convertToFormat(Format, flags)` porte le même texte que la variante `&` émise, donc une entrée du journal nomme une signature qui *existe* dans l'arbre. À retenir pour le budget de rejets par module (mécanisme B) |

#### F1.2 — Sous-cause révélée : le type de retour `auto`

Accepter les ref-qualifiers a débloqué des déclarations qui n'arrivaient jamais jusqu'au *builder*, et
l'une d'elles ne compilait pas : `QJsonObject::asKeyValueRange() &` renvoie `auto` (6.12 le déclare en
`&`, `const &`, `&&` et `const &&` — pas `&&`-seul, contrairement à ce que disait la première lecture).

| | |
| --- | --- |
| Mécanisme | `auto` est classé **spécificateur de stockage** (`parser.cpp`, `parseStorageClassSpecifier`), donc aucun type de retour ne suit ; le *builder* type alors le résultat comme **la classe englobante** |
| Preuve | Sonde isolée (classe minimale, `auto` avec et sans ref-qualifier) : `ProbeClass _q_result = _q_self->deducedLvalue();`. Arbre P-B : `QJsonObject _q_result = _q_self->asKeyValueRange();` → `C2440` |
| Portée | Défaut **antérieur et indépendant** de P-B : il couvre aussi le `template <typename> static constexpr auto qt_create_metaobjectdata();` que `Q_OBJECT` plante dans chaque classe |
| Correctif livré | Refus nommé `DeducedReturnType` (« Return type is deduced ('auto'), not nameable »), avant le refus `&&`, pour que les quatre variantes de `asKeyValueRange` soient attribuées à leur vraie cause |
| Coût | 0 régression : aucune entrée de l'arbre P-C n'est refusée (vérifié entrée par entrée), `asKeyValueRange` n'avait jamais été émis |

#### F1.3 — Ce que le correctif ne peut pas récupérer

`QImage::colorTransformed` et `QImage::convertedToColorSpace` prennent `const QColorTransform &` /
`const QColorSpace &`. Ces deux classes **n'ont aucun wrapper et ne figurent dans aucun typesystem** :
les fonctions sont donc refusées en « unmatched argument type », ref-qualifier ou non. Même famille que
F3 (déclarations manquantes), cause distincte.

> Reste à faire sur F1 : mécanisme B (budget de rejets par module), qui exige d'abord que
> `regen-all.ps1` conserve un journal **par module** et que ce journal **nomme son module** — aujourd'hui
> `write_reject_log_file` ouvre en `WriteOnly`, donc chaque module tronque le journal du précédent et
> seule la dernière section lue est fiable.

### F2 — Fonctions `static` classées **en instance**

**État au 19/10/2026** : **corrigé et mesuré**. Les formes « classe QUuid » et « définition de template »
l'étaient déjà par P-C (`523015b`) — `QUuid.createUuid`, `createUuidV3/V5` sont statiques. Le correctif
ci-dessous ferme la dernière forme (`constexpr static inline`) et **a révélé une seconde cause**, qui doit
être corrigée avec lui.

| | |
| --- | --- |
| Perdu | `QPoint::dotProduct`, `QPointF::dotProduct` : joignables en instance seulement |
| Preuve générée | `qtscript_QPoint.cpp:116` : `_q_self->dotProduct(_q_arg0, _q_arg1)` ; `qtscript_QQuaternion.cpp:491` : `QQuaternion::dotProduct(...)` |
| Preuve runtime (6.12) | `QPoint.dotProduct` = `undefined` ; `p1.dotProduct(p1, p2)` = `11` ; `QQuaternion.dotProduct` = `function` |
| Journalisé | **oui**, depuis P-E : `** WARNING dropped member declaration at …` (une ligne par membre) |

Micro-sonde `micro-f2.h`, DOM du parseur (attribut `static` ajouté par P-E) :

| Forme | `static` |
| --- | --- |
| `constexpr static inline int f(...) { }` — **forme exacte de `qpoint.h:58`** | **perdu** |
| `inline static constexpr int f(...) { }` | **perdu** |
| `constexpr static int f(...)` | conservé |
| `static constexpr inline int f(...) { }` | conservé |
| `static inline int f(...) { }` / `static int f(...)` | conservé |
| `int f(...) { }` (témoin) | instance, correct |

Cause isolée dans `parser/parser.cpp` :

1. `parseDeclarationInternal()` consomme les *function-specifiers* **avant** le storage-class (L3521),
   puis le storage-class (L3527) ;
2. la seconde passe de *function-specifiers* n'a lieu que si la première n'a rien trouvé (L3529,
   `if (hasStorageSpec && !hasFunSpec)`), et celle d'après le type que si `!hasFunSpec` (L3674) ;
3. dans `constexpr static inline int f(...)`, `inline` reste donc en tête de déclarateur :
   `parseTypeSpecifier()` échoue, la déclaration est **abandonnée** (d'où l'avertissement), et la reprise
   de la boucle de membres s'arrête sur `inline` (`tokenCanStartDeclaration`) sans le consommer ;
4. la boucle re-parse `inline int f(...) { }` comme une déclaration neuve : le nœud produit porte
   `storage_specifiers = <vide>`, le `static` appartenant à la tentative abandonnée. Le membre est lié
   **en instance** — c'est le seul signe de la perte, avec l'abandon de la première tentative.
5. **Confiné** (`micro-f2b.h`) : le membre suivant garde son `static` ; il n'y a pas de désynchronisation.

Exposition : **56 lignes** des neuf modules portent un *function-specifier* des deux côtés de `static` ;
pour les classes publiques liées, seuls `QPoint`/`QPointF::dotProduct` sont concernés (les formes
`constexpr static …` conservent `static`, et les autres cas sont des en-têtes privés).

Correctif appliqué (`parser/parser.cpp`) : les deux gardes `!hasFunSpec` sont retirées — les
*function-specifiers* sont désormais consommés avant le storage-class, entre le storage-class et le type,
et après le type. Ces boucles ne consomment que des tokens qui appartiennent nécessairement à la tête de
déclaration, donc les formes qui fonctionnaient déjà ne changent pas.

**Seconde cause, révélée par la première.** Le drapeau `friend` survit maintenant au parseur.
`constexpr friend inline QVector2D operator+(QVector2D, QVector2D)` — une définition *hidden friend*
écrite dans la classe — était auparavant abandonnée puis re-parsée **sans** `friend` : elle arrivait dans
`abstractmetabuilder.cpp` comme un opérateur ordinaire, et le bloc de conversion (L2235) la liait en
**appel libre** (`Friendly`). Avec `friend` conservé, la garde `if (function_item->isFriend()) return 0;`
(L1958) la rejette **avant** ce bloc : 15 membres (`equals` et `operator_add/subtract/multiply/divide` de
`QVector2D/3D/4D`) disparaissaient et la surface tombait à 8 837. La garde laisse donc passer les **seules
définitions hidden friend** (inline, nom d'opérateur, premier argument **non pointeur**), et tout `friend`
non converti est abandonné à la sortie du bloc de conversion. Le filtre « non pointeur » n'est pas
cosmétique : sans lui le build casse sur `qevent.h:454`,
`friend inline bool operator==(QKeyEvent *, QKeySequence::StandardKey)`, que le bloc de conversion
transformerait en `operator==(*_q_self, clé)` — un objet passé là où un pointeur est attendu.

Mesure (6.12, neuf modules) : surface **8 852 → 8 861** (`+12 / -3`), `test.ps1` **22/22** vert.

* 3 **reclassements** : `QDir::listSeparator`, `QPoint[F]::dotProduct` passent d'instance à **static**
  (les trois entrées « instance » disparaissent — c'est la correction elle-même) ;
* 15 membres de `QVector2D/3D/4D` **revenus à l'identique** après la seconde cause ;
* 9 membres **nouvellement liés**, même mécanisme appliqué uniformément : `QQuaternion` (`equals` + 4
  opérateurs) et `QMargins` (4 opérateurs) — cohérent avec la liste d'opérateurs déjà codée en dur dans
  `classgenerator.cpp:746` (`QQuaternion`, `QMargins` y figurent) ;
* sonde runtime : `QPoint.dotProduct` = `function`, `p1.dotProduct` = `undefined`, `v.operator_add(w)`,
  `v.equals(w)`, `q1.equals(q2)`, `m.operator_add(...)` tous corrects.

Rappel du diagnostic initial (formes QUuid, corrigées depuis) : P1/P2/P3 conservaient `static`, P4
(`constexpr static inline`) et P5 (définition de template) le perdaient, et P7 — `friend constexpr` en
`private` + bloc `#if 0/#else` + macro variadique + `public:` + deux statics — ne perdait que le premier,
soit exactement la forme de `quuid.h:236-258`.

### F3 — Classes **nouvelles ou nouvelles bases** Qt 6.11/6.12 non déclarées dans les typesystems

| Perdu | Preuve | Correctif |
| --- | --- | --- |
| `QReadWriteLock::lockForRead/lockForWrite/unlock` | En 6.12 les déclarations de `QReadWriteLock` sont derrière `#if QT_CORE_REMOVED_SINCE(6, 11)` (faux) : elles ne vivent plus que dans la **base** `QBasicReadWriteLock` (`qreadwritelock.h:18`), absent du DOM ; le DOM du parseur ne contient **aucun** `QReadWriteLock::lockForRead`. `tryLockForRead(int)` reste lié ✅. Au contraire `QBasicMutex` **est** déclaré (`typesystem_core.xml:1766`) et généré | `<object-type name="QBasicReadWriteLock"/>` |
| `QFileIconProvider::type()`, `IconType`, `Options/setOptions/options` | En 6.12 `QFileIconProvider : public QAbstractFileIconProvider` (QtGui) qui porte `enum IconType`, `virtual QString type(const QFileInfo&) const`, `icon(IconType)`, `Option/Options`. `QAbstractFileIconProvider` n'est déclaré nulle part → non généré. La ligne `<enum-type name="QFileIconProvider::IconType"/>` (`typesystem_widgets.xml:154`) est donc **morte** | `<object-type name="QAbstractFileIconProvider"/>` + ses enums |
| `QTimeZone::OffsetData` (+ `offsetData/nextTransition/previousTransition`) | `struct OffsetData` présent dans le flux (L351759) ; classe **rejetée** et fonctions **journalisées** (`mjb_rejected_classes.log`, `mjb_rejected_functions.log`) | `<value-type name="QTimeZone::OffsetData"/>` (sinon assumer et documenter) |
| `QPinchGesture` | `<object-type name="QPinchGesture"/>` est **commenté** (`typesystem_widgets.xml:3926`) alors que ses enums restent déclarés (L366, L3935 → lignes mortes) | décommenter, ou nettoyer les enums |

### F4 — Enums **renommés** en Qt 6.9 (alias `using`) et QFlags déclarés sous le nom du typedef

| Typesystem (actuel) | Qt 6.12 | Preuve | Correctif |
| --- | --- | --- | --- |
| `typesystem_core.xml:583` `<enum-type name="QLibraryInfo::LibraryLocation"/>` | `enum LibraryPath { … }` + `using LibraryLocation = LibraryPath;` | `LibraryPath` **est** dans le DOM, `LibraryLocation` non | déclarer `QLibraryInfo::LibraryPath` |
| `typesystem_gui.xml:95` `<enum-type name="QLineF::IntersectType"/>` | `enum IntersectionType { … }` + `using IntersectType = IntersectionType;` (flux L427801-427802) | `IntersectType` introuvable | déclarer `QLineF::IntersectionType` |
| `typesystem_core.xml:580` `<enum-type name="QFileDevice::MemoryMapFlags"/>` | `enum MemoryMapFlag { … }` + `Q_DECLARE_FLAGS(MemoryMapFlags, MemoryMapFlag)` | `MemoryMapFlag` est l'enum, `MemoryMapFlags` le typedef | `<enum-type name="QFileDevice::MemoryMapFlag" flags="QFileDevice::MemoryMapFlags"/>` (forme des voisins `FileHandleFlag`, `Permission`) |

**Silencieux** dans les trois cas (aucun rejet) : le générateur ne dit pas qu'une déclaration de
typesystem ne correspond à rien dans le DOM. C'est le point que P-E doit fermer.

### F5 — Aplatissement `QList<T>` **incomplet** sur `QPolygon`, `QPolygonF`, `QItemSelection`, `QXmlStreamAttributes`

**État au 19/10/2026, après P-D1 (`2019394`)** : les membres à type d'élément sont liés. Mesure sur
l'arbre 6.12 : surface **8 822 → 8 852** noms, **+30, 0 perdu** — `QPolygon` +8, `QPolygonF` +8,
`QItemSelection` +8 (`assign`, `endsWith`, `fill`, `prepend`, `push_back`, `push_front`, `replace`,
`startsWith`) et `QXmlStreamAttributes` +6 (les mêmes moins `push_back`/`push_front`, curés). Sonde
runtime sur 6.12 : `append`, `prepend`, `push_back`, `push_front`, `startsWith`, `endsWith`, `replace`,
`fill`, `assign` et `mid` exécutés, `assign` vérifié sur le conteneur **et** sur son retour.

Diagnostic initial (conservé pour la trace) — perdu : `at`, `indexOf`, `lastIndexOf`, `contains`,
`prepend`, `push_back/push_front`, `fill`, `replace`, `startsWith`, `endsWith`, `takeFirst/takeLast`,
`front/back`. Conservé : `constFirst`, `constLast`, `takeAt`, `mid`, `sliced`, `value`, `count`, `size`,
`swapItemsAt`, …

Cause racine isolée par comparaison du jeu conservé/écarté avec `qlist.h` 6.12 :

| Écarté | Déclaration 6.12 | Type en jeu |
| --- | --- | --- |
| `at` | `const_reference at(qsizetype) const noexcept` (L512) | typedef interne `const_reference` |
| `front/back` | `reference` / `const_reference` | typedefs internes |
| `takeFirst/takeLast` | `value_type takeFirst()` (L637) | typedef interne |
| `prepend/push_*/replace/fill/startsWith/endsWith` | `parameter_type` / `rvalue_ref` | typedefs internes |
| `indexOf/lastIndexOf/contains` | déclarés dans la **base** `QListSpecialMethodsBase<AT>` (L45-52) et ramenés par `using` (L671+, L660-662) | base + `AT` |

| Conservé | Déclaration 6.12 | Type en jeu |
| --- | --- | --- |
| `constFirst/constLast` | `const T &` explicite | type littéral |
| `takeAt` | `T takeAt(qsizetype)` | type littéral |
| `mid/sliced` | `QList<T>` | instanciation résolue |

→ L'aplatissement ne résolvait pas les **typedefs internes du conteneur** et ne suivait pas les `using`
de la base. `QList::at` restait donc invisible en script sur `QPolygon`. L'arbre de référence (Qt 5.15
dont le `QList` déclarait `const T &at(int)`, `int indexOf(const T&, int)`…) les a tous : c'est un
**écart de capacité du générateur**, pas une propriété de Qt 6.

#### Ce qui a été corrigé

* **F5 option B (`84eb2d7`)** — suivi des `using` de base et résolution des alias à portée classe :
  `at`, `takeFirst/takeLast`, `front/back`, `value_type`, `const_iterator`/`iterator` reviennent.
* **P-D1 (`2019394`)** — deux mécanismes, deux causes distinctes :
  1. **`parameter_type` / `rvalue_ref` → le type d'élément.** `qlist.h` déclare ces deux alias contre
     `DataPointer::parameter_type` / `DataPointer::rvalue_ref`, cibles **inatteignables** (représentation
     privée). Aucune déclaration ne nomme l'élément : le membre est écarté en « unmatched argument type »,
     puis l'aplatissement le perd sans trace. Fix : quand la cible de l'alias est
     `DataPointer::parameter_type` **et** que l'alias prend un paramètre de template, enregistrer à sa place
     la forme de la branche `Q_QDOC` de `qlist.h` (`const T &`). La garde est sûre : pour les quatre types
     d'élément en jeu (`QPoint`, `QPointF`, `QItemSelectionRange`, `QXmlStreamAttribute`),
     `QArrayDataPointer::pass_parameter_by_value = is_arithmetic || is_pointer || is_enum` est **faux**,
     donc `const T&` est la cible réelle ; les seules autres classes déclarant ces alias
     (`qproperty.h`, `qarraydataops.h`) visent une autre cible et ne peuvent pas déclencher la garde.
     `rvalue_ref` reste **non lié**, volontairement : `T &&` est un paramètre rvalue seul, et le filtre
     rvalue du binder s'exécute sur le déclarateur (où l'alias est encore écrit par son nom) — le lier
     produirait un appel avec une lvalue qui ne compilerait pas. Ces jumeaux sont désormais **nommés**
     (« Only callable with an rvalue (rvalue reference parameter, or '&&' ref-qualifier) », classe
     `RequiresRvalue`) : 7 rejets sortent d'« unmatched argument type ».
  2. **Retour écrit avec le nom de classe injecté.** `qlist.h` déclare `QList &assign(qsizetype, parameter_type)`
     — le retour porte le **nom de la classe** et non `T`. L'instanciation remplie ne couvrait que les
     arguments : le wrapper généré ne compilait pas (`QList& assign(...)`). `fillContainerInstantiation()`
     est maintenant appliquée au retour **et** aux arguments dans `inheritTemplate()`. C'est la panne qui
     a révélé le second défaut ; sans elle le correctif n'aurait pas compilé.

#### Ce qui reste dehors, et pourquoi

| Membre | Raison |
| --- | --- |
| `insert` (6 entrées) | rejet **curé** `<rejection class="QList" function-name="insert"/>` (`typesystem_core-qtscript.xml`) — décision, pas perte |
| `QXmlStreamAttributes::push_back/push_front` | également curés : les déclarations de typesystem jusqu'ici **mortes** (le membre n'existait pas) sont devenues actives avec P-D1 |
| `QXmlStreamAttributes::append(QXmlStreamAttribute)` | règle de *shadowing par nom* de l'aplatissement : la sous-classe déclare déjà `append`, le membre copié est ignoré (idem `QPolygon::swap`). Tracé dans `mjb_lost_template_functions.log`, antérieur à P-D1 |
| `contains`, `indexOf`, `lastIndexOf` | **templates de fonction** : déclarés dans la base `QListSpecialMethodsBase<AT>` (`template <typename AT> qsizetype indexOf(const AT &t)`) et ramenés par `using`. Le générateur ne les voit qu'après instanciation du template **de fonction** au site d'appel : c'est P-D2, hors du périmètre de l'aplatissement (perte définitive inscrite, voir la sous-section P-D2 ci-dessous) |
| `QPolygon::mid()` | joignable, mais rend un `Array` JavaScript (retour `QList<T>` converti par `qScriptValueFromSequence`), pas un wrapper : `mid(1)[0]` fonctionne, les membres de `QList` n'existent pas sur le résultat |

#### P-D2 — Templates de **fonction** membre : perte définitive (inscrite le 20/10/2026)

`contains`, `indexOf`, `lastIndexOf` sont déclarés **une seule fois** dans la base
`QListSpecialMethodsBase<AT>` sous la forme `template <typename AT> qsizetype indexOf(const AT &)`,
ramenés dans chaque liste par `using`. Un template de *fonction* membre n'a de signature complète qu'au
site d'appel : le générateur raisonne sur la **déclaration** parse, sans instanciation implicite, et le
typesystem n'a pas d'ancrage où demander l'instanciation (`<rejection class="..." function-name="..."/>`
ne voit pas de nom). Aucun contournement côté générateur : il faudrait un compilateur réel pour
instancier au site. Perte d'**appelabilité**, pas de parité : les retours liste (`QPolygon::mid()`…)
arrivent en `Array` JavaScript, où `Array.prototype.indexOf/includes` rendent le même service.

Inscrite ici comme **définitive** pour qu'un audit ultérieur ne la re-découvre pas en « F » : à traiter
comme `insert` (rejet curé : décision explicite, pas une panne).

#### F5.1 — Surcharges **identiques** après aplatissement (révélé par P-D1 sur 6.8.3)

Mesure 6.8.3 (artefact CI `qtscriptgenerator-qt6.8.3-generated-cpp` du run `39300797413`) : surface
**8 755 → 8 785**, **+30 / 0 perdu** — mêmes gains que 6.12. Le run 6.12 est vert ; le run 6.8 LTS
échoue à `collections-smoke` : `QPolygon::fill(): could not find a function match`.

Cause isolée : sur la ligne 6.8.3, la classe aplatie porte `fill` (et `value`) **deux fois** —
`qtscript_QPolygon.cpp` 6.8.3 émet deux branches `argumentCount() == 2` de garde identique, là où
6.12 n'en émet qu'une :

| | 6.12 | 6.8.3 |
| --- | --- | --- |
| table d'introspection `fill` | `QPoint t, qsizetype size` | `QPoint t, qsizetype newSize` **+** `QPoint t, qsizetype size` |
| table d'introspection `value` | `qsizetype i` + `qsizetype i, QPoint defaultValue` | idem **+** `qsizetype i, QPoint defaultValue` (deux fois) |
| branche 2-arg émise | un corps inconditionnel | deux corps gardés par `qMetaTypeId<qsizetype>() == argument(1).toVariant().userType()` |

La garde ne peut pas réussir : un nombre JavaScript est un `double`, pas un `long long` — d'où
« could not find a function match ». Les deux entrées sont la **déclaration en classe**
(`fill(parameter_type t, qsizetype size = -1)`, `qlist.h:574`) et sa **définition hors classe**
(`QList<T>::fill(parameter_type t, QList<T>::size_type newSize)`) : mêmes types après substitution,
noms de paramètres différents. `_ScopeModelItem::declaredFunction()` apparie par `isSimilar()`, qui
compare les types d'arguments **par chaîne**, donc l'orthographe décide : sur 6.8.3 la déclaration dit
`qsizetype` et la définition dit `QList<T>::size_type`, les deux restent distinctes ; sur 6.12 les deux
disent `qsizetype` (`qlist.h` L650 et L994) et fusionnent. Le générateur est le même des deux côtés :
la divergence est entièrement dans le texte des en-têtes.

Correctif appliqué (`b5c9dee`) : dans `inheritTemplate()`, ne pas recopier un membre dont la
`minimalSignature()` est déjà présente parmi les copies héritées par la même boucle ; le doublon part
dans `mjb_lost_template_functions.log` avec la raison. Le contrôle de *shadowing* existant ne pouvait
pas le faire : il ne compare que les **noms** et itère un instantané pris avant la boucle.

Mesures : sur 6.12, `com_trolltech_qt_gui` est **byte-identique** avant/après (246 fichiers) et
`qtscript_QPolygon.cpp` garde sa branche 1-arg et sa seule branche 2-arg — là les deux déclarations
fusionnent déjà, la déduplication ne se déclenche donc jamais. Le mécanisme est prouvé sur une sonde qui
déclare la forme 6.8.3 dans un `QList` (A/B sur la même entrée, deux binaires du générateur) :

| | avant | après |
| --- | --- | --- |
| branches `fill` | 1-arg + **deux** 2-arg, gardes identiques `qMetaTypeId<qlonglong>() == argument(1).toVariant().userType()` | 1-arg + une 2-arg |
| journal | vide | `MPoly <- List::fill() : duplicate signature 'fill(int,qlonglong)' (declared twice in the template)` |

Effet attendu sur 6.8.3 : 6 membres redeviennent joignables (`fill`/`value` sur `QPolygon`,
`QPolygonF`, `QItemSelection`), à confirmer par le prochain run.

### F6 — Rejets **journalisés** restants (32 items, tous traçables)

Exemples et raisons (détail par module dans `audit-all\<module>\mjb_rejected_functions.log`) :

| Rejet | Raison |
| --- | --- |
| `QByteArray::cbegin/cend` | retour `const char *` (pointeur brut non liable) — perte assumée |
| `QColor::name`, `QRegion::rects`, `QGradient::stops/setStops`, `QTextLayout::textOption`, `QNetworkReply::rawHeaderPairs`, `QHostAddress::parseSubnet` | type de retour/argument non instanciable en script (`QList<QPair<…>>`, `QVector<QRect>`, `std::pair<…>`) |
| `QVariantAnimation::keyValues/setKeyValues` | API **supprimée** en Qt 6 (dérive, rien à faire) |
| `QMenu::addAction` (templates), `QAction::menu` | formes génériques/curées, couvertes autrement |

Ces pertes sont **visibles** (contrairement à F1–F5 et F7) : c'est la matière de la gate B.

### F7 — Ordre des enums **non déterministe** : arbre non reproductible et valeurs fausses

**État au 19/10/2026** : cause isolée, corrigé et commité (`6bc3c1a`), validé par la paire CI verte sur
`ca0ec36`.

| | |
| --- | --- |
| Symptôme 1 | deux regens **identiques** (même binaire, même Qt, même typesystem) donnent des arbres différents : 46 fichiers widgets ne diffèrent que par l'ordre des `Q_DECLARE_METATYPE(...)` |
| Symptôme 2 | dans l'un des ordres, `QSizePolicy::Policy` perd ses valeurs : les 7 énumérateurs sortent à 0 et sont émis comme **alias de `Fixed`** (`QSizePolicy.Minimum === 0`), alors que Qt donne 0/1/3/4/5/7/13 |
| Preuve | `QT_HASH_SEED=0` rend deux runs identiques au byte ; deux graines (0 et 1) reproduisent chacune une des deux variantes |
| Cause | `_ScopeModelItem::enums()` renvoyait `_M_enums.values()` d'un `QHash` ⇒ ordre à graine aléatoire par processus. `figureOutEnumValuesForClass()` calcule les valeurs dans cet ordre et résout `Policy::Minimum = GrowFlag` dans `m_enum_values`, qui vaut encore 0 tant que `PolicyFlag` n'a pas été traité |
| Correctif | `parser/codemodel.{h,cpp}` : conserver la liste des enums en **ordre de déclaration** à côté du `QHash` et la renvoyer depuis `enums()` (remplacement en place si un nom est réinséré) |
| Effet mesuré | deux graines (0 et 1) ⇒ arbre widgets **identique au byte** ; enums dans l'ordre `PolicyFlag, Policy, ControlType` ; `QSizePolicy` émet ses 7 valeurs |

Ce n'est pas une perte d'API, mais une **non-reproductibilité** de l'arbre généré (donc des artefacts CI) et,
dans un ordre sur deux, une **valeur d'enum fausse**. Les deux sont invisibles dans la mesure de surface
(qui ne compte que les noms de membres) et dans les 22 suites actuelles.

Reste ouvert : les autres `QHash::values()` du DOM (`functions()`, `typeAliases()`, `variables()`)
peuvent encore changer l'**ordre** d'émission, et la résolution d'un alias `using` d'enum reste sensible
à l'ordre d'inscription (voir F8) : l'orthographe `QIODeviceBase::OpenMode` vs
`QAbstractSocket::QIODeviceBase::OpenMode` — deux orthographes valides C++ — varie d'un regen à l'autre
pour l'orthographe seule, jamais pour les valeurs. `QT_HASH_SEED=0` dans les scripts et la CI rend les
artefacts comparables en attendant un tri explicite.

**Leçon de méthode** : la surface ne voit **pas les enums**. `QFileIconProvider::IconType`, que j'avais
classé « mort » sur cette seule base, est en réalité émis (`QFileIconProvider.IconType` = 7 valeurs au
runtime) ; c'est `QPinchGesture::WhatChange/ChangeFlag` qui ne produit rien (classe non liée).

---

### F8 — Valeurs d'enum **inter-classes** : dépendantes de l'ordre de traversal (`QLocalSocket`)

**État au 20/10/2026** : cause isolée, corrigé et commité (`423dfac`), validé (6 regens non seedés +
chaîne complète 22 suites + assertion dans `network-smoke.js`).

| | |
| --- | --- |
| Symptôme | `QLocalSocket::LocalSocketError` sort parfois réduit à **1 valeur** (`ConnectionRefusedError`) + **10 alias « same numeric value »** : `QLocalSocket.ServerNotFoundError === 0` au lieu de 2. Sur 10 regens `network` non seedés : **2 arbres cassés**, sans une seule ligne « duplicate/unmatched » dans les 3,4 Mo de journaux |
| Surface du problème | 38 blocs d'alias « same numeric value » dans l'arbre publié, dont beaucoup **légitimes** (défauts C++) ; les exposées sont les références inter-classes — `QLocalSocket::LocalSocketError/State` (11 + n énumérateurs vers `QAbstractSocket::*`) est la seule du catalogue à 6.12, déclarée dans `typesystem_network-common.xml` (L27-28) |
| Preuve | sonde `f8.h` à graine 0 : `SRC::E{X = 5}` / `REF::F{Y = SRC::X, W = 5}` ⇒ Y = 5 (collision → 1 alias) ou Y = 0 selon l'ordre de traitement des classes ; avec les noms réels, 3 runs sur 6 tombaient à 0 |
| Cause | chaîne : (1) `_ScopeModelItem::classes()` renvoyait `QHash::values()` ⇒ l'ordre de traversal est à graine (F7 l'a corrigé pour `enums()` seulement) ; (2) `figureOutEnumValuesForClass()` ne recurse que dans `baseClass()`, jamais dans la classe **cité par** `E = AutreClasse::X` ; (3) `m_enum_values` est rempli au fil du traversal, valeurs à 0 tant que leur classe n'est pas passée ⇒ une référence lue avant sa cible vaut **0**, tous les énumérateurs collident sur le premier |
| Correctif | `423dfac` : (B) résolution **à la demande** — `AbstractMetaEnumValue` connaît désormais son enum (back-pointer) ; quand une référence vise une valeur pas encore calculée, `figureOutEnumValue()` résout la classe propriétaire avant de lire sa valeur, avec un jeu « classes en cours » qui interrompt un cycle d'alias impossible en C++ mais rapporté au lieu de récurser ; (C) comme F7, `classes()` renvoie l'**ordre de déclaration** (remplacement en place à la réinsertion, retrait dans `removeClass()`) |
| Effet mesuré | 6 regens `network` non seedés ⇒ 62 fichiers, les 11 valeurs de `LocalSocketError` à chaque run, 0 alias ; sonde ⇒ Y = 5 dans 6 runs sur 6 ; chaîne complète 6.12 (9 modules, build, 22 suites) et surface **8 861** inchangée ; `network-smoke.js` cale désormais les valeurs (`UnknownSocketError = -1`, `ServerNotFoundError = 2`, `OperationError = 19`, égalité avec l'énumérateur source) |

La valeur du correctif : une passe « chance » sur deux produits des **numéros faux** dans un binding
compilé et voté — le genre de panne que ni la surface (noms seulement), ni les suites existantes
(n'avaient pas ces constantes), ni les journaux (silencieux) ne voyaient. C'est aussi le premier correctif
de l'audit à jouer la partie **runtime** : l'assertion nouvelle échoue sur un arbre cassé par
construction (une seule valeur), donc elle verrouille la ligne.

---

## 4. Faux positifs retirés (ne rien corriger)

| Item « perdu » | Réalité 6.12 |
| --- | --- |
| `QDateTime::startOfDay/endOfDay` | En 6.12 ce sont **`QDate::startOfDay/endOfDay`** (3 surcharges) et le fork les lie correctement (`qtscript_QDate.cpp`) ; le port les expose *en plus* sur `QDateTime` (confort). **Pas une perte** |
| `QFileIconProvider::type` | N'existe plus sur la classe en 6.12 : déplacé dans la base `QAbstractFileIconProvider` → voir F3 (manque de **déclaration**), pas un bug d'émission |
| `QMutex::lock` | Fourni par `QBasicMutex` ✅ (déclaré au typesystem) |
| `QMouseEvent::position`, `QPointingDevice`, `QInputDevice` | Hiérarchie Qt 6.9+ (`QSinglePointEvent`), présents ✅ |
| `QCryptographicHash::Algorithm.Sha3_256` | Alias de `RealSha3_256` ✅ |
| `QMenuBar/QToolBar::addAction` | Forme générique héritée ✅ |
| `QPaintDevice` | Simple changement de module ✅ |
| `QVariantAnimation::keyValues` | Supprimé en Qt 6 (dérive) |
| Helpers curés (`valueOf`, `operator_less`, `operator_negate`…) | Choix délibéré ✅ |

Dérive Qt assumée : `ABSENT-DE-QT` 69, `DERIVE-QT` 141, `CLE-ENUM-DERIVE-QT` 23 — rien à faire, sinon les
surveiller avec la gate de surface.

---

## 5. Le vrai problème : le **silence**

Les familles F1–F5 (et F7) n'apparaissaient dans **aucun** journal :

* F1 : fonction absente du DOM (parseur), rien dans `mjb_rejected_functions.log` → **corrigé** : les
  deux refus sont désormais nommés (F1.1, F1.2) ;
* F2 : `static` perdu au parseur — désormais **journalisé** par P-E (« dropped member declaration »,
  une ligne par membre) ; cause isolée, corrigé et mesuré ;
* F3 : classes absentes du typesystem → absentes du modèle (les rejets de `OffsetData`/`PinchGesture`
  sont l'exception) ;
* F4 : déclaration de typesystem **sans correspondance** dans le DOM → ignorée en silence ;
* F5 : signature d'aplatissement non résolue → écartée en silence ;
* F7 : ordre des enums tiré d'un `QHash` → ni journal, ni test, ni mesure de surface ne pouvaient le
  voir ; **corrigé** (l'ordre de déclaration est conservé) ;
* F8 : référence d'enum inter-classes lue à 0 tant que sa classe cible n'est pas venue dans un ordre de
  traversal à graine → **numéros faux** dans l'arbre compilé, sans journal ni test ; **corrigé**
  (résolution à la demande), et verrouillé au runtime dans `network-smoke.js`.

C'est exactement le mécanisme déjà documenté dans `regen-all.ps1:71-78` (`INCLUDE` non vide ⇒ 4 classes
QtCore — dont `QByteArray` — perdues sans un mot dans le log, 80 classes au lieu de 84). **`exit=0`,
« 9/9 modules OK » et 13 967 rejets journalisés ne prouvent donc pas la complétude.**

Conséquences sur le plan :

* **A — contrat de surface** : la référence est la surface **6.12** (et l'artefact **6.8** comme second
  point de mesure), **pas le port**. Passez à un diff **par signature** (l'instrument actuel écrase les
  surcharges). La paire 6.8/6.12 est stable (14 retraits / 103 ajouts) : tout écart inattendu saute aux yeux.
  **Livre le 20/10/2026** : `tools/surface-scan.ps1` (lire, dans chaque wrapper, la table des noms qu'il
  émet), `tools/surface-contract.ps1` et **un contrat par ligne Qt** — les deux lignes émettent une
  surface volontairement différente (ajouts/retraits côté Qt : `QBasicReadWriteLock` n'existe pas en
  6.12, `QJsonValue::toStringView` n'existe pas en 6.8.3…), un seul fichier rejetterait à tort les 71
  noms de la bonne ligne : `generator/surface-contract.json` **6.12** (457 classes, 8 861 membres) et
  `generator/surface-contract-qt683.json` **6.8.3** (456 classes, 8 790 membres), semé depuis l'arbre
  de l'artefact du run vert `39475844690`. Check dans les deux workflows **après la génération, avant
  la gate budget** : un nom du contrat absent de l'arbre régénéré échoue, **nommé** (`gui : QPolygon.fill`,
  classe entière: `QLocalSocket (whole class)`), en 30 s au lieu d'attendre la compilation. Les
  ajouts sont annoncés sans bloquer et entrent dans le contrat via `-Update` (diff git relu) ; une
  perte n'est réécrite qu'avec `-Update -Force`. Complément exact du budget : le budget ne voit
  que ce que le générateur journalise, le contrat voit ce qu'il **émet** — c'est lui qui ferme la
  classe de panes silencieuses (F1–F5, F7, F8).
* **B — gate des rejets par module** : le journal est réécrit par chaque module, donc seul le dernier
  survivait — la gate n'avait pas d'entrée fiable. **Livré (en cours de validation CI)** :
  `regen-all.ps1` déplace les six journaux dans `<output>\logs\<module>\` après chaque module (et les
  efface avant, pour qu'un module qui n'écrit rien n'hérite pas du précédent) ; `tools/reject-budget.ps1`
  compare ces neuf dossiers à `generator/reject-budget.json` (une borne par module et par motif), échoue
  sur toute croissance et sur tout motif inconnu, et signale les bornes devenues lâches. Amorçage par
  `-Update`, qui ne fait que **monter** la borne : d'abord la ligne 6.12, puis la 6.8.3, pour qu'un seul
  fichier serve les deux. À brancher dans les deux workflows **après la génération et avant la
  compilation** (un membre perdu échoue en 30 s au lieu d'attendre 20 min de build), et les journaux par
  module sont joints à l'artefact `generated-cpp`.
  Le journal reste utile aux deux autres volets : déclaration de typesystem introuvable dans le DOM
  (`enum-type`, `object-type`, `flags=`), classe de base absente, signature d'aplatissement non résolue.
* **C — suites engendrées depuis l'arbre généré** : plutôt que des sondes choisies à la main pour
  F1–F5, trois suites sont **reconstruites à chaque run** à partir de `generated_cpp/`, donc sans
  référence versionnée et sur les deux lignes Qt :
  * `tools/surface-coverage.ps1` → 457 classes / 8 861 membres joignables (livré avec A) ;
  * `tools/enum-values.ps1` → **473 enums, 4 823 constantes** : un oracle C++ (`printf` de chaque
    constante castée `long long`, compilé avec `cl` + en-têtes Qt) dit ce que **Qt** répond, la suite
    compare avec la valeur que les bindings exposent au runtime (repli `Class.Key` puis
    `Class.Enum.Key`). L'identité d'une constante est lue dans le **nom du tableau**
    (`qtscript_<Class>_<Enum>_values`) et non dans le type déclaré : les enums globaux (`QtMsgType`)
    sont attachés par le binding à `Global`. Un écart **valeurs/clés** est une erreur bloquante, pas un
    cas à ignorer — c'est exactement la forme de F8 (1 valeur, 11 clés) ;
  * `tools/construction-sweep.ps1` → les 457 classes doivent être des fonctions enregistrées, porter un
    prototype et — quand le générateur émet un constructeur sans argument (381 classes) — construire un
    objet par `new Class()`. La détection se borne au `case 0` de `qtscript_<Class>_static_call`,
    jusqu'au `break;` : `QItemSelection::maxSize` (case 2, 0 argument) a servi de faux positif. Un
    constructeur peut tuer le process sans lever : la suite écrit la classe en cours dans un fichier de
    progression (`logs/construction-smoke.progress.txt`) avant chaque tentative.
  Contrôles négatifs joués : valeur d'enum falsifiée ⇒ `Forward = 0, Qt says 1` ; arbre façon F8 ⇒
  `LocalSocketError (1 values for 11 keys)` ; classe renommée ⇒ `QPolygonTypo is not a constructor
  function` ; constructeur inexistant déclaré présent ⇒ `QItemSelection new QItemSelection() threw`.
  Rappel : dans `qs_eval`, `throw new Error(...)` (jamais `throw "…"`) ; `print()` n'écrit que sur
  stdout du process, inutilisable comme assertion.
* **D — `test.ps1` en découverte automatique** (`tests/*-smoke.js`) : inchangé.

---

## 6. Propositions (ordre conseillé, état au 21/10/2026)

| # | Correctif | Fichier(s) | Portée | Risque | Preuve attendue |
| --- | --- | --- | --- | --- | --- |
| **P-E** | Instrumentation : journaliser les 5 pertes silencieuses (`qWarning`/log + compteurs) ; ajouter le drapeau `static` au dump `--ast-to-xml` | `abstractmetabuilder.cpp`, `parser/binder.cpp`, `asttoxml.cpp` | diagnostic seul | nul (aucun effet sur la sortie) | un regen 6.12 liste F1–F5 ; un regen 6.8 en liste moins |
| **P-A** | Typesystem : `QFileDevice::MemoryMapFlag` + `flags=`, `QLibraryInfo::LibraryPath`, `QLineF::IntersectionType`, `QBasicReadWriteLock`, `QAbstractFileIconProvider` (+`IconType`, `Option/Options`), `QTimeZone::OffsetData` ; puis retirer les **deux enums morts** de `QPinchGesture` (`WhatChange`, `ChangeFlag` — classe non liée). `QFileIconProvider::IconType` **n'est pas** mort : il est émis, et la mesure de surface ne voit pas les enums | `typesystem_core.xml`, `typesystem_gui.xml`, `typesystem_widgets.xml` | 7 lignes | **nul** (miroir de `QBasicMutex`) | `F…` présents au runtime ; `test.ps1` vert ; paire CI 6.8/6.12 |
| **P-C** | Parseur : propager `static` (formes P4/P5/P7) — **complété le 19/10/2026** : les deux gardes `!hasFunSpec` tombent et la garde `friend` de `abstractmetabuilder` laisse passer les définitions *hidden friend* | `parser/parser.cpp`, `abstractmetabuilder.cpp` | 3 formes | faible (reclasser des méthodes) | **fait** : surface 8 852 → 8 861, `QPoint[F]::dotProduct` statiques, 22 suites vertes |
| **P-G** | Parseur : conserver l'**ordre de déclaration** des enums (`_ScopeModelItem::enums()` renvoyait `QHash::values()`) | `parser/codemodel.{h,cpp}` | F7 | faible | **fait** : deux graines de hachage ⇒ arbre identique au byte ; `QSizePolicy` à ses 7 valeurs ; l'axe
        `classes()` est traité en complément par F8 (`423dfac`, valeurs inter-classes) |
| **P-B** | Parseur : accepter les ref-qualifiers (`const &` lié, `&&` écarté) — **livré `d0b1a62`**, avec le refus nommé des retours `auto` (F1.2) | `parser/parser.cpp`, `parser/ast.h`, `parser/binder.cpp`, `parser/codemodel.*`, `abstractmetabuilder.*` | famille F1 | moyen (déduplication des surcharges) | **fait** : `QByteArray` 13 + `QImage` 4 présents, 0 perdu, 22 suites vertes sur 6.12 |
| **P-D** | Aplatissement : résoudre les typedefs internes (`parameter_type`, `rvalue_ref`, `value_type`, `AT`) et suivre les `using` de base — **livré `84eb2d7` (option B) + `2019394` (P-D1)** ; P-D2 (templates de fonction) est désormais inscrit comme perte définitive (§F5/P-D2) | `abstractmetabuilder.cpp` (`AbstractMetaBuilder`, remplissage des instanciations) | F5 | moyen | **fait** : +30 noms, 0 perdu sur 6.12 ; `collections-smoke.js` 122 références, 0 insatisfaite |
| **P-F** | Aplatissement : ne recopier qu'une fois un membre dont la `minimalSignature()` est déjà héritée — **livré `b5c9dee`** (F5.1) | `abstractmetabuilder.cpp` (`inheritTemplate()`) | F5.1 | faible (le doublon est déclaré tel quel dans le template) | **fait** : sonde A/B, 3 branches → 2 ; 6.12 byte-identique |

Ordre : **P-E → P-A → P-C → P-B → P-D → P-F**. Livrés : P-E/P-A (`3f0899e`), F5 en option B (`84eb2d7`),
P-B (`d0b1a62`, assertion versionnée `1286571`), P-D1 (`2019394`, suite `1074a13`), P-C (`a54e49b`),
P-G (`6bc3c1a`), P-F (`b5c9dee`), F8 (résolution à la demande B + `classes()` ordonnée C, `423dfac`).
Après chaque étape :
regen 6.12 locale → `build.ps1` → `test.ps1` → push `qt6` →
vérification de la paire CI 6.8.3 + 6.12 (les artefacts CI 6.8/6.12 servent de mesure de non-régression).

---

## 7. État des dépôts et traçabilité

* `D:\Codez\qtscriptgenerator_for_5.15.2` : branche `qt6`. Correctifs livrés : `3f0899e` (P-E/P-A),
  `84eb2d7` (F5 option B), `10a3338`, `1f47cb3`, `523015b` (récupération à portée classe), `d0b1a62`
  (ref-qualifiers + refus `auto`), `1286571` (assertion versionnée), `2019394` (P-D1 : alias
  `parameter_type` + retour instancié), `1074a13` (suite collections), `a54e49b` (P-C : `static` + *hidden
  friends*), `c1982e5` (deux enums morts de `QPinchGesture`), `6bc3c1a` (P-G : ordre de déclaration des
  enums), `a23b479` (mécanisme B : un journal de rejets par module + budget), `b5c9dee` (P-F : F5.1),
  `ca0ec36` (ce rapport), `423dfac` (F8 : valeurs d'enum inter-classes, résolution à la demande +
  `classes()` en ordre de déclaration), `70a1fd5` (gate budget dans les deux workflows), `002cfad`
  (mécanisme A : `tools/surface-scan.ps1` + `tools/surface-contract.ps1` + contrat `457/8 861/9`
  branché après la génération dans les deux workflows), `ed08c44` (la ligne 6.8.3 a son contrat :
  `generator/surface-contract-qt683.json`, 456/8 790, semé depuis l'artefact du run vert
  `39475844690`), `b2ac740` (C4715 : les wrappers de virtuelles pures sont émis avec
  `Q_UNREACHABLE_RETURN(<défaut>)` après le `qFatal` — Qt ne marque `qFatal` noreturn que hors MSVC
  (`qlogging.h` L67-70), les 85 avertissements de build disparaissent sans toucher au code curé ;
  surface inchangée, 25 suites vertes), `T1/T2` (deux suites engendrées : `tools/enum-values.ps1`,
  473 enums / 4 823 constantes contre un oracle C++ compilé sur les en-têtes Qt ; `tools/construction-sweep.ps1`,
  457 classes enregistrées + 381 constructeurs sans argument exécutés ; switchs `-SkipEnumValues` et
  `-SkipConstructionSweep` dans `test.ps1` ; en local sur 6.12 : 22 suites + les trois suites engendrées
  vertes, 159 s pour la configuration `Release`).
  Seuls `.cmake/` et `.cmake612-rwdi/` restent non suivis (préexistants).
* Les runs de sonde et de génération ont réécrit `generator/.preprocessed.tmp`, `generator/mjb_*.log`
  et `generated_cpp/` (tous **ignorés** par git).
* Dépôt bindings : lecture seule (HEAD détaché `ce3c93e`, `git status` propre).
* Sondes et sorties brutes : `C:\Users\julie\AppData\Local\Temp\opencode\qtscript_probe\`
  (`surface-*.txt`, `diff-only-port.txt`, `audit-*.csv/txt`, `audit-all\`, `ci-683\`, `ast-min.xml`,
  `micro*-out\`, `min-out\`).
* Reproduction : `regen-all.ps1 -QtPrefix 'C:\Qt\6.12.0\msvc2022_64' …` puis
  `build.ps1` / `test.ps1` ; sondes minimales via
  `generator.exe <sonde.h> <sonde-ts.txt> --output-directory=<dir>` (`QTDIR`, `PATH` Qt, `INCLUDE=''`).
* Rejeter un correctif d'aplatissement exige de reconstruire le générateur avant de mesurer :
  `generator\build-generator.ps1 -QtPrefix 'C:\Qt\6.12.0\msvc2022_64'` (incrémental, ~15 s), puis
  `regen-all.ps1 -Modules <module> -OutputRoot <dir>` ; comparer le module généré au précédent **au
  byte**. `-ApplyCurated` exige core/printsupport/svg/xml et échoue sur un regen mono-module : mesurer
  sans lui.
* Les journaux de rejets sont désormais **par module** (`regen-test\logs\<module>\mjb_*.log`, repris
  dans l'artefact CI) et vérifiés par `tools\reject-budget.ps1` contre `generator\reject-budget.json` ;
  le budget n'est semé que pour 6.12, celui de 6.8.3 attend un artefact qui publie ces journaux.
* Une sonde d'aplatissement doit s'appuyer sur un template **connu du `TypeDatabase`** : `setupInheritance()`
  ne cherche la base que dans `m_templates`, alimenté par `typeEntry()->isContainer()`. Une classe
  `MList` inventée n'hérite de rien ; une sonde qui redéfinit `QList` dans son propre en-tête (sans
  inclure QtCore) déclenche bien l'aplatissement.
