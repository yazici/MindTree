/*
    FRG Shader Editor, a Node-based Renderman Shading Language Editor
    Copyright (C) 2011  Sascha Fricke

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "graphics_node_socket.h"

#include "QMenu"
#include "QInputDialog"
#include "QPainter"
#include "QGraphicsSceneMouseEvent"
#include "QGraphicsTextItem"
#include "QTextDocument"
#include "QStringList"
#include "QApplication"
#include "QDrag"
#include "QMimeData"

#include "data/nodes/data_node_socket.h"
#include "data/frg.h"
#include "data/nodes/data_node.h"
#include "graphics_node.h"
#include "source/graphics/base/vnspace.h"
#include "data/project.h"

using namespace MindTree;

SocketTypeAction::SocketTypeAction(DSocket *socket, socket_type t, QObject *parent)
    : socket(socket), type(t), QAction(parent)
{
    connect(this, SIGNAL(triggered()), this, SLOT(setType()));
}

void SocketTypeAction::setType()    
{
    socket->setType(type);
}

SocketButtonContainer::SocketButtonContainer(VNSocket *socket)
    :socket(socket), changeTypeButton(0), unlinkButton(0)
{
    if(socket->getData()->getDir() == IN)
        setPos(-18, 0);
    else
        setPos(10, 0);
    changeTypeButton = new SocketChangeTypeButton(socket);
    changeTypeButton->hide();
    changeTypeButton->setParentItem(this);
    if(socket->getData()->getDir() == IN && !socket->getData()->getArray()){
        unlinkButton = new SocketUnlinkButton(socket);
        unlinkButton->hide();
        unlinkButton->setParentItem(this);
    }
    setParentItem(socket);
    setAcceptHoverEvents(true);
}

SocketButtonContainer::~SocketButtonContainer()
{
}

void SocketButtonContainer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
}

QRectF SocketButtonContainer::boundingRect() const
{
    return QRectF(-40, -10, 60, 30);
};

void SocketButtonContainer::hoverEnterEvent(QGraphicsSceneHoverEvent *event)    
{
    if(unlinkButton)unlinkButton->show();
    changeTypeButton->show();
    QGraphicsItem::hoverEnterEvent(event);
}

void SocketButtonContainer::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)    
{
    if(unlinkButton)unlinkButton->hide();
    changeTypeButton->hide();
    QGraphicsItem::hoverLeaveEvent(event);
}

SocketChangeTypeButton::SocketChangeTypeButton(VNSocket *socket)
    : socket(socket)
{
    if(socket->getData()->getDir() == IN)
        setPos(0, -5);
    else
        setPos(0, -5);
    setAcceptHoverEvents(true);
    setParentItem(socket);
    setZValue(zValue() + 3);

    createMenu();
}

SocketChangeTypeButton::~SocketChangeTypeButton()
{
}

void SocketChangeTypeButton::createMenu()    
{
    typeMenu = new QMenu();
    typeMenu->addActions(createActions());
}

QList<QAction*> SocketChangeTypeButton::createActions()    
{
    QList<QAction*> actions;
    QStringList names;
    names << "Normal" << "Vector" << "Float" << "Color" << "Point" << "String" << "Variable" << "Condition"
                    <<  "Matrix" << "Object" << "Polygon" << "Integer" << "Vec2" << "Mat2" << "Mat3" << "Sampler1D" << "Sampler2D"
                    << "Sampler3D" << "SamplerCube" << "Sampler1DShadow" << "Sampler2DShadow";
    for(int i=0; i<21; i++) {
        QAction *action = new SocketTypeAction(socket->getData(), (socket_type)i, (QObject*)socket);
        action->setText(names.at(i));
        actions.append(action);
    }
    return actions;
}

QRectF SocketChangeTypeButton::boundingRect() const
{
    return QRectF(0, 0, 10, 10);
};

void SocketChangeTypeButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    //painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    if(isUnderMouse())
        painter->setBrush(QBrush(QColor(120, 120, 120, 200)));
    else
        painter->setBrush(QBrush(QColor(100, 100, 100, 150)));
    painter->drawPixmap(0, 0, QPixmap(":/unlink.svg"));
    painter->drawEllipse(0, 0, 10, 10);    
}

void SocketChangeTypeButton::mousePressEvent(QGraphicsSceneMouseEvent *event)    
{
    QGraphicsItem::mousePressEvent(event);
    typeMenu->exec(event->screenPos());
}

SocketViewButton::SocketViewButton(VNSocket *socket)
    : socket(socket), menu(new QMenu)
{
    setPos(10, -5);
    setAcceptHoverEvents(true);
    setZValue(zValue() + 3);
}

SocketViewButton::~SocketViewButton()
{
}

QRectF SocketViewButton::boundingRect() const
{
    return QRectF(0, 0, 10, 10);
};

void SocketViewButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    //painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    if(isUnderMouse())
        painter->setBrush(QBrush(QColor(120, 120, 120, 200)));
    else
        painter->setBrush(QBrush(QColor(100, 100, 100, 150)));
    painter->drawPixmap(0, 0, QPixmap(":/view.svg"));
    painter->drawEllipse(0, 0, 10, 10);    
}

void SocketViewButton::mousePressEvent(QGraphicsSceneMouseEvent *event)    
{
    QGraphicsItem::mousePressEvent(event);
}


SocketUnlinkButton::SocketUnlinkButton(VNSocket *socket)
    : socket(socket)
{
    setPos(-10, -5);
    setAcceptHoverEvents(true);
    setZValue(zValue() + 3);
}

SocketUnlinkButton::~SocketUnlinkButton()
{
}

QRectF SocketUnlinkButton::boundingRect() const
{
    return QRectF(0, 0, 10, 10);
};

void SocketUnlinkButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    //painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->scale(0.25, 0.25);
    if(isUnderMouse())
        //painter->setBrush(QBrush(QColor(120, 120, 120, 200)));
        painter->drawPixmap(-15, -5, QPixmap(":./linked_hover.svg"));
    else
        painter->drawPixmap(-15, -5, QPixmap(":./linked.svg"));
        //painter->setBrush(QBrush(QColor(100, 100, 100, 150)));
    //painter->drawEllipse(0, 0, 10, 10);    
}

void SocketUnlinkButton::mousePressEvent(QGraphicsSceneMouseEvent *event)    
{
    QGraphicsItem::mousePressEvent(event);
    socket->getData()->toIn()->clearLink();
}

VNSocket::VNSocket(DSocket *data, VNode *parent)
	: data(data), width(SOCKET_WIDTH), height(SOCKET_HEIGHT), socketNameVis(0), drawName(true), visible(true)
{
	//setCacheMode(ItemCoordinateCache);
    setAcceptDrops(true);
    setAcceptHoverEvents(true);
    setZValue(3);
    //setFlag(QGraphicsItem::ItemClipsChildrenToShape, false);
    setFlag(ItemIsSelectable, false);
//    setGraphicsEffect(new QGraphicsDropShadowEffect);
    createContextMenu();
	createNameVis();
    setParentItem(parent);
    socketButtons = new SocketButtonContainer(this);

    connect(data, SIGNAL(nameChanged(QString)), parent, SLOT(updateNodeVis()));
}

VNSocket::~VNSocket()
{
    delete contextMenu;
}

DSocket* VNSocket::getData() const
{
    return data;
}

QColor VNSocket::getColor()    
{
    return color;
}

void VNSocket::createNameVis()
{
	//create Name
	socketNameVis = new QGraphicsTextItem(data->getName());
    socketNameVis->setDefaultTextColor(QColor(255, 255, 255));
	socketNameVis->setParentItem(this);
	socketNameVis->setZValue(zValue()+.2);
	socketNameVis->setY(-3 * height/4);
    QFont font = socketNameVis->font();
    font.setPointSize(9);
    socketNameVis->setFont(font);
	if(data->getDir() == OUT)
    {
        socketNameVis->setX(-width/2);
		socketNameVis->setX(socketNameVis->x()-getSocketNameVisWidth());
    }
	else
		socketNameVis->setX(width/2);
}

void VNSocket::setDrawName(bool draw)    
{
    drawName = draw;
    if(!draw && socketNameVis)
        killNameVis();
    updateNameVis();
}

void VNSocket::setVisible(bool vis)    
{
    if(!vis && visible) setZValue(zValue() - 0.1);
    else if(vis && !visible)setZValue(zValue() + 0.1);
    visible = vis;
}

void VNSocket::setBlockContextMenu(bool block)    
{
    blockContextMenu = block;
    if(block)
    {
        delete contextMenu;
        contextMenu = 0;
    }
}

void VNSocket::updateNameVis()    
{
   if(drawName && !socketNameVis)
       createNameVis(); 
}

void VNSocket::killNameVis()    
{
    if(!socketNameVis)
        return;
    socketNameVis->setParentItem(0);
    auto space = (VNSpace*)scene();
    if(space)
        if(space->items().contains(socketNameVis))space->removeItem(socketNameVis);
    delete socketNameVis;
    socketNameVis = 0;
}

int VNSocket::getSocketNameVisWidth() const
{
    if(!socketNameVis)
        return 0;
	return socketNameVis->document()->size().width();
}

void VNSocket::createContextMenu()
{
    contextMenu = new QMenu;
    QAction *changeNameAction = contextMenu->addAction("Rename Socket");
    QAction *changeTypeAction = contextMenu->addAction("change Type");

    connect(changeNameAction, SIGNAL(triggered()), this, SLOT(changeName()));
    connect(changeTypeAction, SIGNAL(triggered()), this, SLOT(changeType()));
}

void VNSocket::changeName()
{
    bool ok;
    QString newname;
    newname = QInputDialog::getText(0, "Change Socket Name", "New Name", QLineEdit::Normal, "", &ok);
    if(ok)
	{
        if(socketNameVis)socketNameVis->setPlainText(newname);
        data->setName(newname);
    }
}

void VNSocket::changeType()
{
    socket_type newtype;
    QStringList typelist;
    bool ok;
    typelist<<"Normal" << "Vector" <<"Float"<<"Integer"
            <<"Condition"<<"Color" <<"Point"<<"String"
            <<"Matrix"<<"Object"<<"Polygon"<<"Variable";
    QString newtypestr(QInputDialog::getItem(0, "Change Socket Type", "New Type", typelist, 0, false, &ok));

    if (newtypestr == "Normal")
        newtype = NORMAL;
    else if (newtypestr == "Vector")
        newtype = VECTOR;
    else if (newtypestr == "Float")
        newtype  = FLOAT;
    else if (newtypestr == "Color")
        newtype = COLOR;
    else if (newtypestr == "Point")
        newtype = POINT;
    else if (newtypestr == "String")
        newtype = STRING;
    else if (newtypestr == "Variable")
        newtype = VARIABLE;
    else if (newtypestr == "Condition")
        newtype = CONDITION;
    else if (newtypestr == "Matrix")
        newtype = MATRIX;
    else if (newtypestr == "Object")
        newtype = SCENEOBJECT;
    else if (newtypestr == "Polygon")
        newtype = POLYGON;
    else if (newtypestr == "Integer")
        newtype = INTEGER;

    if(ok)
        data->setType(newtype);
}

int VNSocket::getWidth() const
{
	return width;
}

void VNSocket::setWidth(int newwidth)
{
	width = newwidth;
}

int VNSocket::getHeight() const
{
	return height;
}

void VNSocket::setHeight(int newHeight)
{
	height = newHeight;
}

QRectF VNSocket::boundingRect() const
{
    return QRectF(-width/2, -height/2, width, height); 
};

void VNSocket::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if(!visible)
        return;
    //painter->setRenderHint(QPainter::Antialiasing);
    //painter->setClipRect(option->exposedRect);
    QPen spen;
    spen.setColor(QColor(50, 50, 50));
    switch(data->getType().getType())
    {
    case STRING:
        if (isUnderMouse())
            color = QColor(255, 100, 100);
        else
            color = QColor(130, 30, 30);
        break;
    case POINT:
        if (isUnderMouse())
            color = QColor(0, 255, 255);
        else
            color = QColor(60, 110, 180);
        break;
    case NORMAL:
        if (isUnderMouse())
            color = QColor(0, 255, 255);
        else
            color = QColor(60, 110, 180);
        break;
    case INTEGER:
        if (isUnderMouse())
            color = QColor(0, 255, 220);
        else
            color = QColor(40, 165, 165);
        break;
    case FLOAT:
        if (isUnderMouse())
            color = QColor(0, 255, 0);
        else
            color = QColor(90, 170, 60);
        break;
    case VECTOR:
        if (isUnderMouse())
            color = QColor(0, 255, 255);
        else
            color = QColor(60, 110, 180);
        break;
    case COLOR:
        if (isUnderMouse())
            color = QColor(255, 255, 0);
        else
            color = QColor(180, 180, 50);
        break;
    case CONDITION:
        if (isUnderMouse())
            color = QColor(255, 0, 0);
        else
            color = QColor(140, 20, 20);
        break;
    case SCENEOBJECT:
        if (isUnderMouse())
            color = QColor(255, 170, 0);
        else
            color = QColor(235, 105, 20);
        break;
    case POLYGON:
        if (isUnderMouse())
            color = QColor(255, 150, 170);
        else
            color = QColor(255, 50, 140);
        break;
    case VARIABLE:
        if(isUnderMouse())
            color = QColor(255, 0, 255);
        else
            color = QColor(90, 40, 90);
    };

    painter->setBrush(QBrush(color, Qt::SolidPattern));
    painter->setPen(spen);
    drawShape(painter);
};

void VNSocket::drawShape(QPainter *painter)    
{
    painter->drawEllipse(-width/2, -height/2, width, height);
}

void VNSocket::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() != Qt::LeftButton) {
        QGraphicsItem::mousePressEvent(event);
        return;
    }

    VNSpace *space = (VNSpace*)scene();
    dragstartpos = event->pos();
	if(!space->isLinkNodeMode())
        space->enterLinkNodeMode(this);
    else
        space->addLink(this);
}

void VNSocket::mouseMoveEvent(QGraphicsSceneMouseEvent *event)    
{
    if((event->pos() - dragstartpos).manhattanLength() < QApplication::startDragDistance()) {
        QGraphicsItem::mouseMoveEvent(event);
        return;
    }

    QDrag *drag = new QDrag(event->widget());
    QMimeData *mimeData = new QMimeData;
    QByteArray itemData;
    QDataStream stream(&itemData, QIODevice::WriteOnly);

    stream << data->getType();
    if(data->getDir() == IN)
        mimeData->setData("FRGShaderAuthor/InSocket", itemData);
    else
        mimeData->setData("FRGShaderAuthor/OutSocket", itemData);
    drag->setMimeData(mimeData);
    drag->exec();
}

void VNSocket::dragEnterEvent(QGraphicsSceneDragDropEvent *event)    
{
    VNSpace *space = (VNSpace*)scene();
    if((event->mimeData()->hasFormat("FRGShaderAuthor/OutSocket")
        ||event->mimeData()->hasFormat("FRGShaderAuthor/InSocket"))
      &&!space->isLinkNodeMode())
          space->enterLinkNodeMode(this);

    if((event->mimeData()->hasFormat("FRGShaderAuthor/OutSocket") && data->getDir() == IN
        ||event->mimeData()->hasFormat("FRGShaderAuthor/InSocket") && data->getDir() == OUT)
      &&space->getLinkSocket() != this) {
      event->acceptProposedAction();
    }
}

void VNSocket::dragMoveEvent(QGraphicsSceneDragDropEvent *event)    
{
    VNSpace *space = (VNSpace*)scene();
    if((event->mimeData()->hasFormat("FRGShaderAuthor/OutSocket") && data->getDir() == IN
        ||event->mimeData()->hasFormat("FRGShaderAuthor/InSocket") && data->getDir() == OUT)
        &&space->getLinkSocket() != this)
        event->acceptProposedAction();
}

void VNSocket::dropEvent(QGraphicsSceneDragDropEvent *event)    
{
    VNSpace *space = (VNSpace*)scene();
    QString format = event->mimeData()->formats().first();
    if(!space->isLinkNodeMode())
        return;
    if((event->mimeData()->hasFormat("FRGShaderAuthor/OutSocket") && data->getDir() == IN
        ||event->mimeData()->hasFormat("FRGShaderAuthor/InSocket") && data->getDir() == OUT)
        &&space->getLinkSocket() != this) {
        QByteArray itemData;
        if(data->getDir() == IN)
            itemData = event->mimeData()->data("FRGShaderAuthor/OutSocket");
        else
            itemData = event->mimeData()->data("FRGShaderAuthor/InSocket");

        QDataStream stream(&itemData, QIODevice::ReadOnly);
        int t;
        stream >> t;
        if(DSocket::isCompatible((socket_type)t, data->getType())) {
            space->addLink(this);
            ((VNode*)parentItem())->updateNodeVis();
        }
        else
            space->cancelLinkNodeMode();
    }
    else
        space->cancelLinkNodeMode();
}

void VNSocket::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if(contextMenu)
        contextMenu->exec(event->screenPos());
}

VNASocket::VNASocket(DAInSocket *socket, VNode *parent)
    : VNSocket(socket, parent)
{
}

VNASocket::~VNASocket()
{
}

void VNASocket::drawShape(QPainter *painter)
{
    painter->drawRoundedRect(-getWidth()/2, -getHeight()/2, getWidth(), getHeight(), 2.5, 2.5);
}