#pragma once

namespace ff7r
{
	class Entity
	{
	public:
		Entity();
		Entity(const Entity& _other);
		virtual ~Entity();

		void			SetName(const wstring& _name) { name = _name; }
		const wstring&	GetName() { return name; }

		UINT			GetID() { return id; }
		virtual Entity* Clone() = 0;

	private:
		static UINT Next_ID;

		const UINT  id;
		wstring		name;
	};
}

