#include "game_object.h"
#include "../SiE.h"

GameObject::GameObject(entt::entity e) : BaseObject(e),RegisterStorage(e)
{

}

TransformComponent& GameObject::Transform()
{
	return GetComponent<TransformComponent>();
}

void GameObject::SetHighlightState(bool state)
{
    Storage().m_HighlightState = state;
}

void GameObject::SetHighlightColor(Color color)
{
    Storage().m_HighlightColor = color;
}

Color GameObject::GetHighlightColor()
{
    return Storage().m_HighlightColor;
}



bool GameObject::IsActive()
{
	return Storage().m_Active;
}

bool GameObject::GetHighlightState()
{
	return Storage().m_HighlightState;
}


void GameObject::SetActiveState(bool state)
{
    Storage().m_Active = state;
}




