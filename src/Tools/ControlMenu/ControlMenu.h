#ifndef GTKMM_CONTROLMENU_H
#define GTKMM_CONTROLMENU_H
#pragma once
#include <gtkmm.h>
#include <iostream>
#include <string>
typedef struct rgb_color
{
	int r;
	int g;
	int b;
}rgb_color_t;

class ControlMenu : public Gtk::Window
{
	public:
		ControlMenu();
		virtual ~ControlMenu();

		rgb_color_t getColor();
		//Node
		int Landmark_Mode;
		int Mesh_Mode;
		int AR_Mode;
	protected:
		//Signal handlers:
		void on_button_2D_Menu();
		void on_button_3D_Menu();
		void on_button_AR_Menu();
		void on_button_2D_Detect();
		void on_button_3D_Mesh();
		// void on_button_2D_color_set();
		void on_button_3D_color_set();
		void on_button_AR1_set();
		void on_button_AR2_set();
		void on_button_AR3_set();
		void on_button_AR4_set();
		void on_button_2D_Point_set();
		void on_button_2D_Point_set2();
		void on_button_3D_Point_set();
		void on_button_3D_Point_set2();
		void on_button_3D_Point_set3();
		//Child widgets:
		Gtk::Box m_VBox_Top, m_VBox_2D, m_VBox_3D, m_VBox_2D_Mode, m_VBox_3D_Mode, m_VBox_AR;

		Gtk::Box m_HBox_2D_Detect, m_HBox_3D_Mesh;

		Glib::RefPtr<Gtk::Adjustment> m_adjustment, m_adjustment_digits, m_adjustment_pagesize;

		Gtk::Scale m_VScale;
		Gtk::Scale m_HScale, m_Scale_Digits, m_Scale_PageSize;

		Gtk::Separator m_Separator;

		Gtk::CheckButton m_CheckButton;

		Gtk::Scrollbar m_Scrollbar;

		//Tree model columns:
		class ModelColumns : public Gtk::TreeModel::ColumnRecord
		{
		public:

			ModelColumns()
			{ add(m_col_position_type); add(m_col_title); }

			Gtk::TreeModelColumn<Gtk::PositionType> m_col_position_type;
			Gtk::TreeModelColumn<Glib::ustring> m_col_title;
		};

		ModelColumns m_Columns;
		
		//Child widgets:
		Gtk::ComboBox m_ComboBox_Position; //ComboBox：下拉式選單
		Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
		//CheckButton
		Gtk::CheckButton m_Button_2D_Detect;
		Gtk::CheckButton m_Button_3D_Mesh;
		//RadioButton
		Gtk::RadioButton m_Button_2D_Point, m_Button_2D_Line;
		Gtk::RadioButton m_Button_3D_Point, m_Button_3D_Triangle, m_Button_3D_Line;
		//TotalButton
		Gtk::Button m_Button_2D;
		Gtk::Button m_Button_3D;
		Gtk::Button m_Button_AR;
		//ColorButton
		Gtk::ColorButton m_Button_Color_2D;
		Gtk::ColorButton m_Button_Color_3D;
		//ARButton
		Gtk::Button m_Button_AR1;
		Gtk::Button m_Button_AR2;
		Gtk::Button m_Button_AR3;
		Gtk::Button m_Button_AR4;
		//Color
		// Gdk::Color m_Color_2D;
		Gdk::Color m_Color_3D;

		int count_2D;
		int count_3D;
		int count_AR;


};

#endif //GTKMM_ControlMenu_H
