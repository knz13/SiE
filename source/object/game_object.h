#pragma once
#include "../general/structures.h"
#include "base_object.h"


class GameObjectProperties {
private:

	bool m_Active = true;
	Color m_HighlightColor = Color::Red;
	bool m_HighlightState = false;

	friend class GameObject;
};


namespace GuiLayer {
    class PropertiesView;
}

class TransformComponent;

class GameObject : public BaseObject<GameObject>, public ecspp::RegisterStorage<GameObject,GameObjectProperties> {
public:
	GameObject(entt::entity e);

	TransformComponent& Transform();

	void SetHighlightState(bool state);
	void SetHighlightColor(Color color);
	Color GetHighlightColor();
	bool GetHighlightState();
	bool IsActive();
	void SetActiveState(bool state);
	

	
	
protected:
	

    friend class GuiLayer::PropertiesView;
};



template<typename Component>
class GameComponent : public ecspp::ComponentSpecifier<Component, GameObject> {
public:
    bool IsEnabled() {
        return GetActiveState();
    }
    bool IsVisibleInEditor() {
        return !m_ShouldHideInEditor;
    }
    bool IsRemovable() {
        return m_IsRemovable;
    }
    bool CanBeDisabled() {
        return m_CanBeDisabled;
    }

    virtual void Update(float deltaTime) {};
    

protected:
   

    void HideInEditor(bool state) {
        m_ShouldHideInEditor = state;
    }
    void SetActiveState(bool state) {
        m_BaseComponentActiveState = state;
    }

    void MakeRemovable(bool state) {
        m_IsRemovable = state;
    }

    void MakeAlwaysEnabled(bool state) {
        m_CanBeDisabled = !state;
    }

    bool GetActiveState() {
        return m_BaseComponentActiveState;
    }

private:
    bool m_BaseComponentActiveState = true;
    bool m_ShouldHideInEditor = false;
    bool m_CanBeDisabled = true;
    bool m_IsRemovable = true;


    friend class GameObject;
};