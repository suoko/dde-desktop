#ifndef DESKTOPITEM_H
#define DESKTOPITEM_H

#include "widgets/elidelabel.h"
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

enum DesktopItemType {
    Computer,
    RecycleBin,
    Application,
    File,
    Folder,
    ApplicationGroup,
    Desktop
};

class DesktopItem : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(bool checked READ isChecked WRITE setChecked)
    Q_PROPERTY(bool hover READ isHover WRITE setHover)
public:

    explicit DesktopItem(QWidget *parent = 0);
    explicit DesktopItem(QString icon, QString name, QWidget *parent = 0);
    ~DesktopItem();

    void initUI();
    void initConnect();

    QString getDesktopIcon();
    QString getDesktopName();
    bool isChecked();
    bool isHover();
    DesktopItemType getType();

signals:
    void desktopIconChanged(QString icon);
    void desktopNameChanged(QString name);
    void hoverChanged(bool hover);
    void checkedChanged(bool checked);

public slots:
    void setDesktopIcon(QString icon);
    void setDesktopName(QString name);
    void setChecked(bool checked);
    void setHover(bool hover);
    void setType(DesktopItemType type);

protected:
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);

private:
    QLabel* iconLabel;
    ElidedLabel* nameLabel;
    QString m_desktopIcon;
    QString m_desktopName;
    bool m_checked;
    bool m_hover;
    DesktopItemType m_type = DesktopItemType::Application;
};

#endif // DESKTOPITEM_H