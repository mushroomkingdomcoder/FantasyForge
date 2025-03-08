#include "UserInterface.h"

UserInterface::UserInterface(Graphics& gfx, std::wstring text_font, char2 char_table_dim, int start_char, int layer)
	:
	gfx(gfx),
	layer(layer),
	gfcText(gfx.GetPixelMap(layer).data(), gfx.GetDimensions(layer).GetVStruct(), Image(text_font.c_str()), char_table_dim, start_char),
	pObjects(),
	drawFlag(true)
{}

void UserInterface::AddInterfaces(std::vector<std::unique_ptr<Object>*> ppInterfaces)
{
	for (auto& ppInterface : ppInterfaces)
	{
		assert(ppInterface); assert(*ppInterface);
		pObjects.emplace_back(std::move(*ppInterface));
	}
}

void UserInterface::DisableInterfaces(std::vector<int> indicies)
{
	for (int& index : indicies)
	{
		assert(index >= 0 && index < pObjects.size());
		pObjects[index]->isActive = false;
	}
}

void UserInterface::EnableInterfaces(std::vector<int> indicies)
{
	for (int& index : indicies)
	{
		assert(index >= 0 && index < pObjects.size());
		pObjects[index]->isActive = true;
		drawFlag = true;
	}
}

void UserInterface::DisableAll()
{
	for (auto& pObject : pObjects)
	{
		pObject->isActive = false;
	}
}

void UserInterface::EnableAll()
{
	for (auto& pObject : pObjects)
	{
		pObject->isActive = true;
	}
	drawFlag = true;
}

bool UserInterface::InterfaceIsEnabled(int index) const
{
	assert(index >= 0 && index < pObjects.size());
	return pObjects[index]->isActive;
}

void UserInterface::Update(float time_ellapsed)
{
	int id = 0;
	for (auto& pObject : pObjects)
	{
		if (pObject->update && pObject->isActive)
		{
			pObject->id = id;
			++id;
			if (pObject->update(pObject, time_ellapsed))
			{
				drawFlag = true;
			}
		}
	}
}

void UserInterface::Draw()
{
	if (drawFlag)
	{
		for (auto& pObject : pObjects)
		{
			if (pObject->isActive)
			{
				pObject->Draw(gfx, layer, gfcText);
			}
		}
		drawFlag = false;
	}
}
