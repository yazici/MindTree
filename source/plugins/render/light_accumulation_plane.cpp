#define GLM_SWIZZLE
#include "deferred_renderer.h"
#include "light_accumulation_plane.h"

using namespace MindTree;
using namespace GL;

LightAccumulationPlane::LightAccumulationPlane()
    : PixelPlane("../plugins/render/defaultShaders/deferredshading.frag")
{
}

LightAccumulationPlane::~LightAccumulationPlane()
{
}

void LightAccumulationPlane::setLights(std::vector<std::shared_ptr<Light>> lights)
{
    std::lock_guard<std::mutex> lock(_lightsLock);
    _lights = lights;
}

std::vector<LightPtr> LightAccumulationPlane::getLight() const
{
    std::lock_guard<std::mutex> lock(_lightsLock);
    return _lights;
}
void LightAccumulationPlane::setShadowPasses(std::unordered_map<std::shared_ptr<Light>,
                                             RenderPass*> shadowPasses)
{
    std::lock_guard<std::mutex> lock(_shadowPassesLock);
    _shadowPasses = shadowPasses;
}

std::unordered_map<LightPtr, RenderPass*> LightAccumulationPlane::getShadowPases() const
{
    return _shadowPasses;
}


void LightAccumulationPlane::drawLight(const LightPtr &light,
                                       ShaderProgram *program)
{
    std::lock_guard<std::mutex> lock2(_shadowPassesLock);
    static const float PI = 3.14159265359;
    UniformStateManager states(program);
    states.addState("light.shadow", static_cast<int>(light->getShadowInfo()._enabled));
    states.addState("light.bias", light->getShadowInfo()._bias);
    if(_shadowPasses.find(light) != _shadowPasses.end()) {
        auto shadowPass = _shadowPasses.at(light);
        auto shadowmap = shadowPass->getOutDepthTexture();
        program->setTexture(shadowmap);
        auto shadowcam = shadowPass->getCamera();
        glm::mat4 mvp = shadowcam->getProjection()
            * shadowcam->getViewMatrix();
        states.addState("light.shadowmvp", mvp);
    }
    double coneangle = 360;
    if(light->getLightType() == Light::SPOT)
        coneangle = std::static_pointer_cast<SpotLight>(light)->getConeAngle();

    states.addState("light.pos",
                    glm::vec4(light->getPosition(),
                              light->getLightType() == Light::DISTANT ? 0 : 1));

    glm::vec3 dir(0);
    if (light->getLightType() == Light::DISTANT
        || light->getLightType() == Light::SPOT){
        dir = light->getTransformation()[2].xyz();
    }

    states.addState("light.dir", dir);
    states.addState("light.color", light->getColor());
    states.addState("light.intensity", light->getIntensity());
    states.addState("light.coneangle", coneangle * PI /180);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    MTGLERROR;
}

void LightAccumulationPlane::draw(const CameraPtr& /* camera */,
                                  const RenderConfig& /* config */,
                                  ShaderProgram *program)
{
    glBlendEquation(GL_FUNC_ADD);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);
    std::lock_guard<std::mutex> lock(_lightsLock);
    for (const LightPtr light : _lights) {
        drawLight(light, program);
    }
}
