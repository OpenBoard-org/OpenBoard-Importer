#include <QtGui>
#include "UBUpgradeToOef.h"
#include "UBUpgradeToOefDelegate.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setOrganizationName("Open Education Foundation");
    app.setOrganizationDomain("eo-f.org");
    app.setApplicationName("OpenSankore2OpenBoard");

    UBUpgradeToOefDelegate* upgradeToOefDelegate = new UBUpgradeToOefDelegate();
    new UBUpgradeToOEF(0,upgradeToOefDelegate);

    return app.exec();

}
