// Qt 6 parse root for the script generator (trial regen, qt6 branch).
// Covers the 9 modules built by qtscriptgenerator-qt6:
// core, gui, network, sql, xml (DOM only), widgets, printsupport,
// uitools, svg (+svgwidgets).
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtWidgets/QtWidgets>
#include <QtPrintSupport/QtPrintSupport>
#include <QtNetwork/QtNetwork>
#include <QtSql/QtSql>
#include <QtSvg/QtSvg>
#include <QtSvgWidgets/QtSvgWidgets>
#include <QtXml/QtXml>

#include <QtUiTools/QtUiTools>
