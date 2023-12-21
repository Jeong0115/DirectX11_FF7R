#pragma once


#include "ImGuiMgr.h"
namespace ff7r
{
	class UI
	{
	public:
		UI(const string& _name);
		virtual ~UI();

		virtual void Init() {}
		virtual void Tick() {}
		virtual void FinalTick();
		virtual int RenderUpdate() = 0;

		void SetActive(bool _active) { is_active = _active; }
		bool IsActive() { return is_active; }

		void SetModal(bool _modal) { is_modal = _modal; }
		bool IsModal() { return is_modal; }

		ImVec2 GetPopupPos() { return pos; }
		void SetPopupPos(ImVec2 _pos) { pos = _pos; }

		const string& GetName() { return name; }
		void SetName(const string& _name) { name = _name; }

		const string& GetID() { return id; }

		void SetSize(float _width, float _height) { size = ImVec2(_width, _height); }

		void AddChildUI(UI* _ui)
		{
			_ui->parent_ui = this;
			child_ui.push_back(_ui);
		}

	private:
		string			name;	
		string			id;		

		ImVec2			size;	
		ImVec2			pos;	

		UI*				parent_ui;		
		vector<UI*>		child_ui;	

		bool			is_modal;
		bool			is_active;

		friend class InspectorUI;
		friend class OutlinerUI;
		friend class ContentUI;
	};

}