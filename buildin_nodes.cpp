#include "node.h"
#include "buildin_nodes.h"
#include "shader_space.h"

void    BuildIn::surfaceInput(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    InputNode *surfnode = new InputNode();
    surfnode->setsurfaceInput();
    surfnode->setPos(space->mousePos);
    space->addNode(surfnode);
};

void BuildIn::displacementInput(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    InputNode *dispnode = new InputNode();
    dispnode->setdisplacementInput();
    dispnode->setPos(space->mousePos);
    space->addNode(dispnode);
};

void BuildIn::volumeInput(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    InputNode *volnode = new InputNode();
    volnode->setvolumeInput();
    volnode->setPos(space->mousePos);
    space->addNode(volnode);
};

void BuildIn::lightInput(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    InputNode *lnode = new InputNode();
    lnode->setlightInput();
    lnode->setPos(space->mousePos);
    space->addNode(lnode);
};

void BuildIn::surfaceOutput(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    OutputNode *surfnode = new OutputNode();
    surfnode->setsurfaceOutput();
    surfnode->setPos(space->mousePos);
    space->addNode(surfnode);
};

void BuildIn::displacementOutput(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    OutputNode *dispnode = new OutputNode();
    dispnode->setdisplacementOutput();
    dispnode->setPos(space->mousePos);
    space->addNode(dispnode);
}

void BuildIn::volumeOutput(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    OutputNode *volnode = new OutputNode();
    volnode->setvolumeOutput();
    volnode->setPos(space->mousePos);
    space->addNode(volnode);
}

void BuildIn::lightOutput(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    OutputNode *lnode = new OutputNode();
    lnode->setlightOutput();
    lnode->setPos(space->mousePos);
    space->addNode(lnode);
}

void BuildIn::MaddNode(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    MathNode *node = new MathNode(ADD);
    node->setPos(space->mousePos);
    space->addNode(node);
}

void BuildIn::MSubNode(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    MathNode *node = new MathNode(SUBTRACT);
    node->setPos(space->mousePos);
    space->addNode(node);
}

void BuildIn::MmultNode(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    MathNode *node = new MathNode(MULTIPLY);
    node->setPos(space->mousePos);
    space->addNode(node);
}

void BuildIn::MdivNode(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    MathNode *node = new MathNode(DIVIDE);
    node->setPos(space->mousePos);
    space->addNode(node);
}

void BuildIn::MdotNode(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    MathNode *node = new MathNode(DOTPRODUCT);
    node->setPos(space->mousePos);
    space->addNode(node);
}

void BuildIn::CandNode(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    ConditionNode *node = new ConditionNode(AND);
    node->setPos(space->mousePos);
    space->addNode(node);
}

void BuildIn::CorNode(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    ConditionNode *node = new ConditionNode(OR);
    node->setPos(space->mousePos);
    space->addNode(node);
}

void BuildIn::CnotNode(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    ConditionNode *node = new ConditionNode(NOT);
    node->setPos(space->mousePos);
    space->addNode(node);
}

void BuildIn::CgreaterNode(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    ConditionNode *node = new ConditionNode(GREATERTHAN);
    node->setPos(space->mousePos);
    space->addNode(node);
}

void BuildIn::CsmallerNode(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    ConditionNode *node = new ConditionNode(SMALLERTHAN);
    node->setPos(space->mousePos);
    space->addNode(node);
}

void BuildIn::CeqNode(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    ConditionNode *node = new ConditionNode(EQUAL);
    node->setPos(space->mousePos);
    space->addNode(node);
}

void BuildIn::VColNode(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    ColorValueNode *node = new ColorValueNode;
    node->setPos(space->mousePos);
    space->addNode(node);
}

void BuildIn::VStrNode(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    StringValueNode *node = new StringValueNode;
    node->setPos(space->mousePos);
    space->addNode(node);
}

void BuildIn::VFlNode(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    FloatValueNode *node = new FloatValueNode;
    node->setPos(space->mousePos);
    space->addNode(node);
}

void BuildIn::ContIfNode(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    ContainerNode *node = new ContainerNode(CONDITIONCONTAINER);
    node->setNodeName("Boolean");
    node->setPos(space->mousePos);
    space->addNode(node);
}

void BuildIn::ContForNode(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    ContainerNode *node = new ContainerNode(FOR);
    node->setNodeName("For");
    node->setPos(space->mousePos);
    space->addNode(node);
}

void BuildIn::ContWhileNode(QGraphicsScene *scene)
{
    Shader_Space *space = (Shader_Space*)scene;
    ContainerNode *node = new ContainerNode(WHILE);
    node->setNodeName("While");
    node->setPos(space->mousePos);
    space->addNode(node);
}


