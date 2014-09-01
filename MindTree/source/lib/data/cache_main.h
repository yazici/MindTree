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

#ifndef CACHE_MAIN_PD1QWTW9

#define CACHE_MAIN_PD1QWTW9

#include "data/nodes/data_node.h"
#include "data/nodes/containernode.h"
#include "mutex"
#include "data/type.h"

namespace MindTree
{
    
class DataCache;

class CacheControl
{
public:
    static DataCache *cache(const MindTree::DoutSocket *socket);
    static bool isCached(const MindTree::DoutSocket *socket);
    static void addCache(const MindTree::DoutSocket *socket, DataCache *cache);
    static DataCache* getCache(const MindTree::DoutSocket *socket);
    static void removeCache(DataCache *cache);
    static void analyse(MindTree::DNode *viewnode, const MindTree::DNode *changedNode);
    static bool isOutDated(const MindTree::DNode *node);

private:
    static std::unordered_map<DataCache*, const MindTree::DoutSocket*>caches;
    static ConstNodeList updateNodes;
};

class LoopCache
{
public:
    LoopCache();
    ~LoopCache();
    void setStep(int step);
    int getStep()const;
    void addData(DataCache *cache);
    DataCache* getCache(const MindTree::DoutSocket *socket=0);
    const MindTree::LoopNode* getNode();
    void free();

private:
    const MindTree::DoutSocket *loopentry;
    std::unordered_map<const MindTree::DoutSocket*, DataCache*> cachedData;
    int stepValue, startValue, endValue;
};

class LoopCacheControl
{
public:
    static LoopCache* loop(const MindTree::LoopNode *node);
    static void del(const MindTree::LoopNode *node);

private:
    static std::unordered_map<const MindTree::LoopNode*, LoopCache*> loops;
};

class AbstractCacheProcessor
{
public:
    typedef TypeDispatcher<NodeType, AbstractCacheProcessor*> CacheList;
    AbstractCacheProcessor();
    virtual ~AbstractCacheProcessor();
    virtual void operator()(DataCache*)=0;
};

class CacheProcessor : public AbstractCacheProcessor
{
public:
    CacheProcessor(std::function<void(DataCache*)> fn);
    virtual ~CacheProcessor();
    void operator()(DataCache* cache);

private:
    std::function<void(DataCache*)> processor;
};

class DoutSocket;
class DataCache
{
public:
    DataCache();
    DataCache(const MindTree::DoutSocket *start);
    DataCache(const DataCache &other);
    DataCache(const DNode *node, DataType t);
    virtual ~DataCache();

    void start(const DoutSocket *socket=nullptr);

    int getTypeID() const;
    DataType getType() const;

    void pushData(Property prop);
    Property getData(int index);
    Property getOutput(DoutSocket* socket = nullptr);
    Property getOutput(int index);
    void setNode(const DNode *n);
    void setType(DataType t);

    const DNode* getNode()const;

    const MindTree::DoutSocket *getStart()const;
    void setStart(const DoutSocket *socket);

    static void addProcessor(SocketType st, NodeType nt, AbstractCacheProcessor *proc);
    static void addGenericProcessor(NodeType nt, AbstractCacheProcessor *proc);
    static const std::vector<AbstractCacheProcessor::CacheList>& getProcessors();
    static std::vector<Property>& getCachedOutputs(const DNode *node);
    static void invalidate(const DNode *node);
    static bool isCached(const DNode *node);


private:
    std::vector<Property>& _getCachedOutputs();

    void cacheInputs();
    void cache(const DinSocket *socket);

    const DNode *node;
    static TypeDispatcher<SocketType, AbstractCacheProcessor::CacheList> processors;
    static AbstractCacheProcessor::CacheList _genericProcessors;
    std::vector<Property> cachedInputs;
    SocketType type;
    const DoutSocket *startsocket;
    static std::unordered_map<const DNode*, std::vector<Property>> _cachedOutputs;
    static std::mutex _cachedOutputsMutex;
};


} /* MindTree */

#endif /* end of include guard: CACHE_MAIN_PD1QWTW9 */
