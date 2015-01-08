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

#define GLM_SWIZZLE
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "QApplication"
#include "QMenu"
#include "QMouseEvent"
#include "QPointF"

#include "iostream"
#include "ctime"

#include "math.h"

#include "../../3dwidgets/widgets.h"
#include "data/raytracing/ray.h"
#include "data/project.h"

#include "../../render/deferred_renderer.h"

#include "viewport.h"

using namespace MindTree;

std::vector<Viewport*> Viewport::_viewports;

Viewport::Viewport() :
    QGLWidget(new GL::QtContext(), nullptr, _viewports.empty() ? nullptr : _viewports[0]),
    defaultCamera(std::make_shared<Camera>()),
    rotate(false), 
    pan(false), 
    zoom(false), 
    selectionMode(false), 
    _showGrid(true), 
    transformMode(0)
{
    _viewports.push_back(this);

    activeCamera = defaultCamera;
    setAutoBufferSwap(false);

    QGLContext *ctx = const_cast<QGLContext*>(context());
    _widgetManager = std::make_shared<Widget3DManager>();
    _renderConfigurator = std::unique_ptr<GL::DeferredRenderer>(new GL::DeferredRenderer(ctx, defaultCamera, _widgetManager.get()));

    setMouseTracking(true);
    doneCurrent();
}

Viewport::~Viewport()
{
    _renderConfigurator->stopRendering();

    _viewports.erase(std::find(begin(_viewports), end(_viewports), this));
}

GL::RenderManager* Viewport::getRenderManager()
{
    return _renderConfigurator->getManager();
}

void Viewport::setData(std::shared_ptr<Group> value)
{
    if(!value) return;

    _renderConfigurator->setGeometry(value);
}

void Viewport::changeCamera(QString cam)    
{
}

void Viewport::resizeEvent(QResizeEvent *)
{
    doneCurrent();
    activeCamera->setProjection((GLdouble)width()/(GLdouble)height());
    activeCamera->setResolution(width(), height());
}

void Viewport::paintEvent(QPaintEvent *)
{
    doneCurrent();
}

void Viewport::showEvent(QShowEvent *)
{
    doneCurrent();
    _renderConfigurator->startRendering();
}

void Viewport::hideEvent(QHideEvent *)
{
    _renderConfigurator->stopRendering();
}

void Viewport::setShowPoints(bool b)    
{
    auto config = _renderConfigurator->getManager()->getConfig();
    config.setDrawPoints(b);
    _renderConfigurator->getManager()->setConfig(config);
}

void Viewport::setShowEdges(bool b)    
{
    auto config = _renderConfigurator->getManager()->getConfig();
    config.setDrawEdges(b);
    _renderConfigurator->getManager()->setConfig(config);
}

void Viewport::setShowPolygons(bool b)    
{
    auto config = _renderConfigurator->getManager()->getConfig();
    config.setDrawPolygons(b);
    _renderConfigurator->getManager()->setConfig(config);
}

void Viewport::setShowFlatShading(bool b)
{
    auto config = _renderConfigurator->getManager()->getConfig();
    config.setShowFlatShaded(b);
    _renderConfigurator->getManager()->setConfig(config);
}

void Viewport::setOption(const std::string &key, Property value)
{
    GL::RenderConfig config = _renderConfigurator->getManager()->getConfig();
    config.setProperty(key, value);
    _renderConfigurator->getManager()->setConfig(config);
}

void Viewport::setShowGrid(bool b)
{
    //grid->setVisible(b);
}

void Viewport::initializeGL()
{
}

void Viewport::resizeGL(int, int)
{
}

void Viewport::paintGL()
{
}

void Viewport::mousePressEvent(QMouseEvent *event)    
{
    glm::ivec2 pos;
    pos.x = event->pos().x();
    pos.y = height() - event->pos().y();

    auto viewportSize = glm::ivec2(width(), height());
    if (_widgetManager->mousePressEvent(activeCamera, pos, viewportSize))
        return;

    lastpos = event->posF();
    if(event->modifiers() & Qt::ControlModifier)
        zoom = true;
    else if (event->modifiers() & Qt::ShiftModifier)
        pan = true;
    else
        rotate = true;
}

void Viewport::mouseReleaseEvent(QMouseEvent *event)    
{
    rotate = false;
    pan = false;
    zoom = false;
    selectionMode =false;
    lastpos = QPointF();
    
    _widgetManager->mouseReleaseEvent();
}

void Viewport::mouseMoveEvent(QMouseEvent *event)    
{
    glm::ivec2 pos;
    pos.x = event->pos().x();
    pos.y = height() - event->pos().y();

    auto viewportSize = glm::ivec2(width(), height());

    if (_widgetManager->mouseMoveEvent(activeCamera, pos, viewportSize))
        return;

    float xdist = lastpos.x()  - event->posF().x();
    float ydist = event->posF().y() - lastpos.y();
    if(rotate)
        rotateView(xdist, ydist);
    else if(pan)
        panView(xdist, ydist);
    else if(zoom)
        zoomView(xdist, ydist);
    lastpos = event->posF();
}

void Viewport::wheelEvent(QWheelEvent *event)    
{
    zoomView(0, event->delta());
}

void Viewport::rotateView(float xdist, float ydist)
{
    if(!activeCamera)
        return;
    glm::mat4 mat;
    glm::vec3 campos = activeCamera->getPosition();
    glm::vec3 center = activeCamera->getCenter();
    glm::vec3 lookat = campos - center;

    mat = glm::rotate(mat, (float)ydist, glm::cross(glm::vec3(0, 1, 0), center - campos));
    mat = glm::rotate(mat, (float)xdist, glm::vec3(0, 1, 0));
    lookat = (mat * glm::vec4(lookat, 1)).xyz();
    activeCamera->posAroundCenter(lookat + center);
}

void Viewport::panView(float xdist, float ydist)    
{
    if(!activeCamera)
        return;
    float lalen = glm::length(activeCamera->getCenter() - activeCamera->getPosition());
    float xtrans = xdist * lalen * 0.005;
    float ytrans = ydist * lalen * 0.005;
    activeCamera->translate(glm::vec3(1, 0, 0) * xtrans);
    activeCamera->translate(glm::vec3(0, 1, 0) * ytrans);
}

void Viewport::zoomView(float xdist, float ydist)    
{
    if(!activeCamera)
        return;
    //glm::vec3 zvec = activeCamera->getCenter() - activeCamera->getPosition();
    activeCamera->moveToCenter(ydist/height());
}