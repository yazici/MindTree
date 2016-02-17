#ifndef RENDER_GR953KUN

#define RENDER_GR953KUN

#include "memory"
#include "mutex"
#include "shared_mutex"
#include "vector"
#include "unordered_set"
#include "glwrapper.h"
#include "../datatypes/Object/object.h"
#include "glm/glm.hpp"

class GeoObject;
namespace MindTree
{
namespace GL
{

class VAO;
class FBO;

class ShaderProgram;

class RenderConfig;

class RenderTree;
class ResourceManager;
class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    void render(const CameraPtr camera, const RenderConfig &config, ShaderProgram* program);

    void setTransformation(glm::mat4 trans);
    glm::mat4 getTransformation();
    glm::mat4 getGlobalTransformation();

    void setParent(Renderer *parent);
    const Renderer* getParent() const;
    Renderer* getParent();
    void addChild(Renderer *child);
    void addChild(std::shared_ptr<Renderer>(child));

    void setVisible(bool visible);

    virtual ShaderProgram* getProgram() = 0;
    void setResourceManager(ResourceManager *manager);

protected:
    friend class ShaderRenderNode;
    ResourceManager* getResourceManager();

    virtual void init(ShaderProgram* program) = 0;
    virtual void draw(const CameraPtr &camera, const RenderConfig &config, ShaderProgram* program) = 0;

    std::vector<std::shared_ptr<Renderer>> _children;

private:
    void _init(ShaderProgram* program);
    std::shared_ptr<VAO> _vao;

    std::atomic<bool> _initialized, _visible;
    glm::mat4 _transformation;
    std::shared_timed_mutex _transformationLock;
    ResourceManager *_resourceManager;

    Renderer* _parent;
};

} /* GL */
} /* MindTree */

#endif /* end of include guard: RENDER_GR953KUN */
