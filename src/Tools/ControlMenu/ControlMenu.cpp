#include "ControlMenu.h"


ControlMenu::ControlMenu()
	:
		//box:顯示垂直或橫向，間距
		m_VBox_Top(Gtk::ORIENTATION_VERTICAL, 0),
		m_VBox_2D(Gtk::ORIENTATION_VERTICAL, 0),
		m_VBox_3D(Gtk::ORIENTATION_VERTICAL, 0),
		m_VBox_2D_Mode(Gtk::ORIENTATION_VERTICAL, 0),
		m_VBox_3D_Mode(Gtk::ORIENTATION_VERTICAL, 0),
		m_VBox_AR(Gtk::ORIENTATION_VERTICAL, 0),

		m_HBox_2D_Detect(Gtk::ORIENTATION_HORIZONTAL, 0),
		m_HBox_3D_Mesh(Gtk::ORIENTATION_HORIZONTAL, 0),

		//TotalButton
		m_Button_2D("-   2D LandMark"),
		m_Button_3D("-   3D Mesh"),
		m_Button_AR("-   AR"),

		// A checkbutton to control whether the value is displayed or not:
		m_CheckButton("hihi", 0),
		m_Button_2D_Detect("2D LandMark"),
		m_Button_3D_Mesh("3D Mesh"),

		//RadioButton
		m_Button_2D_Point("Point"),
		m_Button_2D_Line("Line"),
		m_Button_3D_Point("Point"),
		m_Button_3D_Triangle("Triangle"),
		m_Button_3D_Line("Mesh"),

		//ARButton
		m_Button_AR1("V怪客"),
		m_Button_AR2("無臉男"),
		m_Button_AR3("馬賽克"),
		m_Button_AR4("Hantai"),

		//Node
		Landmark_Mode(0),
		Mesh_Mode(0),
		AR_Mode(0),

		count_2D(0),
		count_3D(0),
		count_AR(0)
		{
			set_title("ControlMenu");
			set_default_size(400, 500);

			//Box+Button setting
			add(m_VBox_Top);
			m_VBox_Top.pack_start(m_Button_2D, Gtk::PACK_SHRINK);
			m_VBox_Top.pack_start(m_VBox_2D, Gtk::PACK_SHRINK);
				m_VBox_2D.pack_start(m_HBox_2D_Detect);
					m_HBox_2D_Detect.pack_start(m_Button_2D_Detect);
					m_HBox_2D_Detect.pack_start(m_VBox_2D_Mode);

						m_VBox_2D_Mode.pack_start(m_Button_2D_Point);
						m_VBox_2D_Mode.pack_start(m_Button_2D_Line);
				// m_VBox_2D.pack_start(m_Button_Color_2D);

			m_VBox_Top.pack_start(m_Button_3D, Gtk::PACK_SHRINK);
			m_VBox_Top.pack_start(m_VBox_3D, Gtk::PACK_SHRINK);
				m_VBox_3D.pack_start(m_HBox_3D_Mesh);
					m_HBox_3D_Mesh.pack_start(m_Button_3D_Mesh);
					m_HBox_3D_Mesh.pack_start(m_VBox_3D_Mode);

						m_VBox_3D_Mode.pack_start(m_Button_3D_Point);
						m_VBox_3D_Mode.pack_start(m_Button_3D_Triangle);
						m_VBox_3D_Mode.pack_start(m_Button_3D_Line);

				m_VBox_3D.pack_start(m_Button_Color_3D);

			m_VBox_Top.pack_start(m_Button_AR, Gtk::PACK_SHRINK);
			m_VBox_Top.pack_start(m_VBox_AR, Gtk::PACK_SHRINK);
				m_VBox_AR.pack_start(m_Button_AR1, Gtk::PACK_SHRINK);
				m_VBox_AR.pack_start(m_Button_AR2, Gtk::PACK_SHRINK);
				m_VBox_AR.pack_start(m_Button_AR3, Gtk::PACK_SHRINK);
				m_VBox_AR.pack_start(m_Button_AR4, Gtk::PACK_SHRINK);
			//TotalButton setting
			m_Button_2D.signal_clicked().connect(sigc::mem_fun(*this,
				&ControlMenu::on_button_2D_Menu));
			m_Button_3D.signal_clicked().connect(sigc::mem_fun(*this,
				&ControlMenu::on_button_3D_Menu));
			m_Button_AR.signal_clicked().connect(sigc::mem_fun(*this,
				&ControlMenu::on_button_AR_Menu));

			//CheckButton setting
			m_Button_2D_Detect.set_can_default(); //????
			m_Button_2D_Detect.grab_default();	//????
			m_Button_2D_Detect.signal_clicked().connect(sigc::mem_fun(*this,
				&ControlMenu::on_button_2D_Detect));
			

			m_Button_3D_Mesh.set_can_default(); //????
			m_Button_3D_Mesh.grab_default();	//????
			m_Button_3D_Mesh.signal_clicked().connect(sigc::mem_fun(*this,
				&ControlMenu::on_button_3D_Mesh));
			
			//RadioButton setting
			m_Button_2D_Line.join_group(m_Button_2D_Point);
			m_Button_3D_Triangle.join_group(m_Button_3D_Point);
			m_Button_3D_Line.join_group(m_Button_3D_Point);
			m_Button_2D_Point.set_sensitive(0);
			m_Button_2D_Line.set_sensitive(0);
			m_Button_3D_Point.set_sensitive(0);
			m_Button_3D_Triangle.set_sensitive(0);
			m_Button_3D_Line.set_sensitive(0);
			m_Button_2D_Point.signal_clicked().connect(sigc::mem_fun(*this,
				&ControlMenu::on_button_2D_Point_set));
			m_Button_2D_Line.signal_clicked().connect(sigc::mem_fun(*this,
				&ControlMenu::on_button_2D_Point_set2));
			m_Button_3D_Point.signal_clicked().connect(sigc::mem_fun(*this,
				&ControlMenu::on_button_3D_Point_set));
			m_Button_3D_Triangle.signal_clicked().connect(sigc::mem_fun(*this,
				&ControlMenu::on_button_3D_Point_set2));
			m_Button_3D_Line.signal_clicked().connect(sigc::mem_fun(*this,
				&ControlMenu::on_button_3D_Point_set3));

			//ColorButton setting
			// m_Color_2D.set_red(255);
			// m_Color_2D.set_blue(255);
			// m_Color_2D.set_green(255);
			// m_Button_Color_2D.set_color(m_Color_2D);
			// m_Button_Color_2D.signal_color_set().connect(sigc::mem_fun(*this,
			// 	&ControlMenu::on_button_2D_color_set));

			m_Color_3D.set_red(65535);
			m_Color_3D.set_blue(65535);
			m_Color_3D.set_green(65535);
			m_Button_Color_3D.set_color(m_Color_3D);
			m_Button_Color_3D.signal_color_set().connect(sigc::mem_fun(*this,
				&ControlMenu::on_button_3D_color_set));

			//ARButton setting
			m_Button_AR1.set_name("AR_CSS");
			m_Button_AR2.set_name("AR_CSS");
			m_Button_AR3.set_name("AR_CSS");
			m_Button_AR4.set_name("AR_CSS");
			m_Button_AR1.signal_clicked().connect(sigc::mem_fun(*this,
				&ControlMenu::on_button_AR1_set));
			m_Button_AR2.signal_clicked().connect(sigc::mem_fun(*this,
				&ControlMenu::on_button_AR2_set));
			m_Button_AR3.signal_clicked().connect(sigc::mem_fun(*this,
				&ControlMenu::on_button_AR3_set));
			m_Button_AR4.signal_clicked().connect(sigc::mem_fun(*this,
				&ControlMenu::on_button_AR4_set));


			//load css
			Glib::RefPtr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
			cssProvider->load_from_path("../dataset/style.css");
			// cssProvider->load_from_path("style.css");			
	
			Glib::RefPtr<Gtk::StyleContext> styleContext = Gtk::StyleContext::create();
	
			//get default screen
			Glib::RefPtr<Gdk::Screen> screen = Gdk::Screen::get_default();
	
			//add provider for screen in all application
			styleContext->add_provider_for_screen(screen, cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

			show_all_children();
		


		}
ControlMenu::~ControlMenu()
{
}

void ControlMenu::on_button_2D_Menu(){
	count_2D++;
	if(count_2D%2==0){
		m_Button_2D.set_label("-   2D LandMark");
		m_VBox_2D.pack_start(m_HBox_2D_Detect);
			m_HBox_2D_Detect.pack_start(m_Button_2D_Detect);
			m_HBox_2D_Detect.pack_start(m_VBox_2D_Mode);

				m_VBox_2D_Mode.pack_start(m_Button_2D_Point);
				m_VBox_2D_Mode.pack_start(m_Button_2D_Line);
		// m_VBox_2D.pack_start(m_Button_Color_2D);
	}
	else{
		m_Button_2D.set_label("+   2D LandMark");
		m_VBox_2D_Mode.remove(m_Button_2D_Line);
		m_VBox_2D_Mode.remove(m_Button_2D_Point);
		m_HBox_2D_Detect.remove(m_VBox_2D_Mode);
		m_HBox_2D_Detect.remove(m_Button_2D_Detect);
		// m_VBox_2D.remove(m_Button_Color_2D);
		m_VBox_2D.remove(m_HBox_2D_Detect);
	}
	show_all_children();
}

void ControlMenu::on_button_3D_Menu(){
	count_3D++;
	if(count_3D%2==0){
		m_Button_3D.set_label("-   3D Mesh");
		m_VBox_3D.pack_start(m_HBox_3D_Mesh);
			m_HBox_3D_Mesh.pack_start(m_Button_3D_Mesh);
			m_HBox_3D_Mesh.pack_start(m_VBox_3D_Mode);

				m_VBox_3D_Mode.pack_start(m_Button_3D_Point);
				m_VBox_3D_Mode.pack_start(m_Button_3D_Triangle);
				m_VBox_3D_Mode.pack_start(m_Button_3D_Line);
		m_VBox_3D.pack_start(m_Button_Color_3D);
	}
	else{
		m_Button_3D.set_label("+   3D Mesh");
		m_VBox_3D_Mode.remove(m_Button_3D_Line);
		m_VBox_3D_Mode.remove(m_Button_3D_Triangle);
		m_VBox_3D_Mode.remove(m_Button_3D_Point);
		m_HBox_3D_Mesh.remove(m_VBox_3D_Mode);
		m_HBox_3D_Mesh.remove(m_Button_3D_Mesh);
		m_VBox_3D.remove(m_Button_Color_3D);
		m_VBox_3D.remove(m_HBox_3D_Mesh);
	}
	show_all_children();
}

void ControlMenu::on_button_AR_Menu(){
	count_AR++;
	if(count_AR%2==0){
		m_Button_AR.set_label("-   AR");
		m_VBox_AR.pack_start(m_Button_AR1, Gtk::PACK_SHRINK);
		m_VBox_AR.pack_start(m_Button_AR2, Gtk::PACK_SHRINK);
		m_VBox_AR.pack_start(m_Button_AR3, Gtk::PACK_SHRINK);
		m_VBox_AR.pack_start(m_Button_AR4, Gtk::PACK_SHRINK);
	}
	else{
		m_Button_AR.set_label("+   AR");
		m_VBox_AR.remove(m_Button_AR4);
		m_VBox_AR.remove(m_Button_AR3);
		m_VBox_AR.remove(m_Button_AR2);
		m_VBox_AR.remove(m_Button_AR1);
	}
	show_all_children();
}

void ControlMenu::on_button_2D_Detect()
{
      if(m_Button_2D_Detect.get_active()){
      	m_Button_2D_Point.set_sensitive(1);
      	m_Button_2D_Line.set_sensitive(1);
      	if(m_Button_2D_Point.get_active()){
		Landmark_Mode=1;
		std::cout<<"Point"<<std::endl;
	}else{
		Landmark_Mode=2;
		std::cout<<"Line"<<std::endl;
	}
      }else{
      	m_Button_2D_Point.set_sensitive(0);
      	m_Button_2D_Line.set_sensitive(0);
      	Landmark_Mode=0;
      }
}

void ControlMenu::on_button_3D_Mesh()
{
      if(m_Button_3D_Mesh.get_active()){
      	m_Button_3D_Point.set_sensitive(1);
      	m_Button_3D_Triangle.set_sensitive(1);
      	m_Button_3D_Line.set_sensitive(1);
      	if(m_Button_3D_Point.get_active()){
		Mesh_Mode=1;
		AR_Mode=0;
		std::cout<<"3D Point"<<std::endl;
	}
	else if(m_Button_3D_Triangle.get_active()){
		Mesh_Mode=2;
		AR_Mode=0;
		std::cout<<"3D Line"<<std::endl;
	}
	else if(m_Button_3D_Line.get_active()){
		Mesh_Mode=3;
		AR_Mode=0;
		std::cout<<"3D CCCCC"<<std::endl;
	}
      }else{
      	m_Button_3D_Point.set_sensitive(0);
      	m_Button_3D_Triangle.set_sensitive(0);
      	m_Button_3D_Line.set_sensitive(0);
      	Mesh_Mode=0;
      }
}

// void ControlMenu::on_button_2D_color_set(){
// 	m_Color_2D = m_Button_Color_2D.get_color();
// }

void ControlMenu::on_button_3D_color_set(){
	m_Color_3D = m_Button_Color_3D.get_color();
}

void ControlMenu::on_button_AR1_set(){
	Mesh_Mode=0;
	AR_Mode=1;
	if(m_Button_3D_Mesh.get_active()){
		m_Button_3D_Mesh.set_active(0);
      		m_Button_3D_Point.set_sensitive(0);
      		m_Button_3D_Triangle.set_sensitive(0);
      		m_Button_3D_Line.set_sensitive(0);
      	}
	std::cout<<"AR1"<<std::endl;
}

void ControlMenu::on_button_AR2_set(){
	Mesh_Mode=0;
	AR_Mode=2;
	if(m_Button_3D_Mesh.get_active()){
		m_Button_3D_Mesh.set_active(0);
      		m_Button_3D_Point.set_sensitive(0);
      		m_Button_3D_Triangle.set_sensitive(0);
      		m_Button_3D_Line.set_sensitive(0);
      	}
	std::cout<<"AR2"<<std::endl;
}
void ControlMenu::on_button_AR3_set(){
	Mesh_Mode=0;
	AR_Mode=3;
	if(m_Button_3D_Mesh.get_active()){
		m_Button_3D_Mesh.set_active(0);
      		m_Button_3D_Point.set_sensitive(0);
      		m_Button_3D_Triangle.set_sensitive(0);
      		m_Button_3D_Line.set_sensitive(0);
      	}
	std::cout<<"AR3"<<std::endl;
}
void ControlMenu::on_button_AR4_set(){
	Mesh_Mode=0;
	AR_Mode=4;
	if(m_Button_3D_Mesh.get_active()){
		m_Button_3D_Mesh.set_active(0);
      		m_Button_3D_Point.set_sensitive(0);
      		m_Button_3D_Triangle.set_sensitive(0);
      		m_Button_3D_Line.set_sensitive(0);
      	}
	std::cout<<"AR4"<<std::endl;
}

void ControlMenu::on_button_2D_Point_set(){
	if(m_Button_2D_Point.get_active()){
		Landmark_Mode=1;
		std::cout<<"Point"<<std::endl;
	}
}

void ControlMenu::on_button_2D_Point_set2(){
	if(m_Button_2D_Line.get_active()){
		Landmark_Mode=2;
		std::cout<<"Line"<<std::endl;
	}
}

void ControlMenu::on_button_3D_Point_set(){
	if(m_Button_3D_Point.get_active()){
		Mesh_Mode=1;
		AR_Mode=0;
		std::cout<<"3D Point"<<std::endl;
	}
}

void ControlMenu::on_button_3D_Point_set2(){
	if(m_Button_3D_Triangle.get_active()){
		Mesh_Mode=2;
		AR_Mode=0;
		std::cout<<"3D Triangle"<<std::endl;
	}
}

void ControlMenu::on_button_3D_Point_set3(){
	if(m_Button_3D_Line.get_active()){
		Mesh_Mode=3;
		AR_Mode=0;
		std::cout<<"3D Line"<<std::endl;
	}
}

rgb_color_t ControlMenu::getColor(){
	rgb_color_t color;

	color.r = m_Color_3D.get_red() / 256;
	color.g = m_Color_3D.get_green() / 256;
	color.b = m_Color_3D.get_blue() / 256;
	return color;

}