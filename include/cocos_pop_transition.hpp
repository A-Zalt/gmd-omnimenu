// Taken from: https://gist.github.com/solodon4/11e7cc53bfa52f207decd7a3fc28fdfc

#include <type_traits>
#include <cocos2d.h>

#pragma once

//------------------------------------------------------------------------------

/// Transition adapter that pops current scene with transition passed as an argument.
///
/// Example:
///     Instead of calling cocos2d::Director::sharedDirector()->popScene() just call:
///
///     cocos2d::Director::sharedDirector()->pushScene(
///         pop_scene_with<cocos2d::TransitionFlipX>::create(1.0f, cocos2d::TransitionScene::Orientation::LEFT_OVER)
///     );
///
/// Note that there is no typical 2nd argument taking the scene for the transition,
/// one simply passes all the arguments they would pass to the transition's create
/// method except for the 2nd argument carrying the scene.
template <typename CCTransition>
struct pop_scene_with : CCTransition
{

    static_assert(std::is_base_of<cocos2d::CCTransitionScene, CCTransition>::value, "Type argument must be a class derived from cocos2d::TransitionScene");

private:

    /// A helper class to access protected member of a director we need
    struct DirectorAccessor : cocos2d::CCDirector
    {
        /// Returns the scene that is previous to the current scene.
        cocos2d::CCScene* getPreviousScene() const
        {
            assert(m_pobScenesStack->count() > 1); // Top is current scene and the user is asking for previous, which must exist
            return dynamic_cast<cocos2d::CCScene*>(m_pobScenesStack->objectAtIndex(m_pobScenesStack->count() - 2));
        }
    };

    /// Catch the end of the transition to pop the actual scene
    virtual void onExit() override
    {
        CCTransition::onExit();
        cocos2d::CCDirector::sharedDirector()->popScene(); // Pop the replica of the previous scene we pushed
        cocos2d::CCDirector::sharedDirector()->popScene(); // Pop the scene we were actually trying to pop
    }

public:

    /// Creator of the scene popper.
    template<typename... Args>
    static pop_scene_with* create(float t, Args&&... args)
    {
        pop_scene_with* pThisScene = new (std::nothrow) pop_scene_with();
        cocos2d::CCScene* pPrevScene = static_cast<DirectorAccessor*>(cocos2d::CCDirector::sharedDirector())->getPreviousScene();

        if (pThisScene && pThisScene->initWithDuration(t, pPrevScene, std::forward<Args>(args)...))
        {
            pThisScene->autorelease();
            return pThisScene;
        }

        CC_SAFE_DELETE(pThisScene);
        return nullptr;
    }
};

//------------------------------------------------------------------------------
