#include "desktopbox.h"
#include "desktopframe.h"
#include "global.h"
#include "widgets/growingtextedit.h"
#include "widgets/elidelabel.h"
#include "desktopitem.h"


DesktopBox::DesktopBox(QWidget *parent) : TranslucentFrame(parent)
{
    m_desktopFrame = new DesktopFrame(this);

    m_textEdit = new GrowingTextEdit(this);
    m_textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_textEdit->setObjectName("Rename");
    m_textEdit->hide();

    connect(signalManager, SIGNAL(renameFinished()), this, SLOT(renameFinished()));
}

DesktopFrame* DesktopBox::getDesktopFrame(){
    return m_desktopFrame;
}

void DesktopBox::handleRename(){
    if (!m_desktopFrame->getLastPressedCheckedDesktopItem().isNull() &&\
            m_desktopFrame->getCheckedDesktopItems().length() == 1){
        DesktopItemPointer pItem = m_desktopFrame->getLastPressedCheckedDesktopItem();
        pItem->setEdited(true);
        if (pItem->getUrl() == ComputerUrl || pItem->getUrl() == TrashUrl){
            return;
        }
        pItem->showFullWrapName();
        m_textEdit->setFocus();
        m_textEdit->move(pItem->mapToGlobal(pItem->getNameLabel()->pos()));
        m_textEdit->resize(pItem->getNameLabel()->size());
        m_textEdit->setFont(pItem->getNameLabel()->font());
        m_textEdit->setText(pItem->getNameLabel()->fullText());
        m_textEdit->show();
        pItem->setChecked(false);
        LOG_INFO() << "=================";
    }
}

void DesktopBox::renameFinished(){
    LOG_INFO() <<  m_textEdit->toPlainText();
    DesktopItemPointer pItem = m_desktopFrame->getLastPressedCheckedDesktopItem();
    emit signalManager->renameJobCreated(pItem->getUrl(), m_textEdit->toPlainText());
    m_textEdit->hide();
}

void DesktopBox::keyPressEvent(QKeyEvent *event){
    bool m_isGridOn = m_desktopFrame->isGridOn();

    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Control){
        m_desktopFrame->setCtrlPressed(!m_desktopFrame->isCtrlPressed());
    }

    if (event->key() == Qt::Key_Escape){
        close();
    }else if (event->key() == Qt::Key_PageUp){
        if (m_currentPage > 0){
            m_currentPage--;
            const QRect availableGeometry = QApplication::desktop()->availableGeometry();
            int desktopWidth = availableGeometry.width();
            int currentX = m_desktopFrame->x() + desktopWidth;
            m_desktopFrame->move(currentX, m_desktopFrame->y());
        }
    }else if (event->key() == Qt::Key_PageDown){
        if (m_currentPage < gridManager->getPageCount() - 1){
            m_currentPage++;
            const QRect availableGeometry = QApplication::desktop()->availableGeometry();
            int desktopWidth = availableGeometry.width();
            int currentX = m_desktopFrame->x() - desktopWidth;
            m_desktopFrame->move(currentX, m_desktopFrame->y());
        }
    }else if (event->key() == Qt::Key_1){
        emit signalManager->gridSizeTypeChanged(SizeType::Small);
    }else if (event->key() == Qt::Key_2){
        emit signalManager->gridSizeTypeChanged(SizeType::Middle);
    }else if (event->key() == Qt::Key_3){
        emit signalManager->gridSizeTypeChanged(SizeType::Large);
    }else if (event->key() == Qt::Key_4){
        emit signalManager->sortedModeChanged(QDir::Name);
    }else if (event->key() == Qt::Key_5){
        emit signalManager->sortedModeChanged(QDir::Size);
    }else if (event->key() == Qt::Key_6){
        emit signalManager->sortedModeChanged(QDir::Type);
    }else if (event->key() == Qt::Key_7){
        emit signalManager->sortedModeChanged(QDir::Time);
    }else if (event->key() == Qt::Key_F1){
        emit signalManager->gridModeChanged(!m_isGridOn);
    }else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_A){
        emit signalManager->keyCtrlAPressed();
    }else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_Delete){
        emit signalManager->trashingAboutToExcute(m_desktopFrame->getCheckedFiles());
    }else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_Up){
        if (m_isGridOn){
            emit signalManager->keyUpPressed();
        }
    }else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_Down){
        if (m_isGridOn){
            emit signalManager->keyDownPressed();
        }
    }else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_Left){
        if (m_isGridOn){
            emit signalManager->keyLeftPressed();
        }
    }else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_Right){
        if (m_isGridOn){
            emit signalManager->keyRightPressed();
        }
    }else if (event->modifiers() == Qt::ShiftModifier && event->key() == Qt::Key_Left){
        if (m_isGridOn){
            emit signalManager->keyShiftLeftPressed();
        }
    }else if (event->modifiers() == Qt::ShiftModifier && event->key() == Qt::Key_Right){
        if (m_isGridOn){
            emit signalManager->keyShiftRightPressed();
        }
    }else if (event->modifiers() == Qt::ShiftModifier && event->key() == Qt::Key_Up){
        if (m_isGridOn){
            emit signalManager->keyShiftUpPressed();
        }
    }else if (event->modifiers() == Qt::ShiftModifier && event->key() == Qt::Key_Down){
        if (m_isGridOn){
            emit signalManager->keyShiftDownPressed();
        }
    }if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_F2){
        handleRename();
    }

    TranslucentFrame::keyPressEvent(event);
}


void DesktopBox::keyReleaseEvent(QKeyEvent *event){
    if (event->modifiers() != Qt::ControlModifier && event->key() == Qt::Key_Control){
        m_desktopFrame->setCtrlPressed(!m_desktopFrame->isCtrlPressed());
    }
    TranslucentFrame::keyReleaseEvent(event);
}


void DesktopBox::closeEvent(QCloseEvent *event){
    LOG_INFO() << "closeEvent";
    event->accept();
}

DesktopBox::~DesktopBox()
{

    LOG_INFO() << "~DesktopBox";
}

