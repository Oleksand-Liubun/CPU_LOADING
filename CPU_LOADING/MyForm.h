#pragma once
#include "CPU_info.h"
#include <iterator>
#include <algorithm>
#include "AboutProgram.h"
#include <string>
#include <list>
#include <windows.h>
#include <tlhelp32.h>
#include <math.h>
#include <ShellAPI.h>
#include <atlstr.h>  

std::list<CPU_info> info(std::list<CPU_info> cpu_info)
{
	return cpu_info;
}

std::list<CPU_info> all_processes;
std::list<CPU_info> real_time_processes;
CPU_info curent_add_process;

struct is_odd {
	bool operator()(const CPU_info &pr_list)
	{
		return curent_add_process._id == pr_list._id;
	}
};

namespace CPU_LOADING {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::IO;
	using namespace System::Diagnostics;

	UINT nOfReturnedIDs;

	struct procStruct {
		std::string name = "def";
		int pid = -500;
		float CPU;
		FILETIME kt, ut;
		FILETIME sit, skt, sut;
		ULONG64 ktd, utd, ktutd;
		ULONG64 skd, sud, sktutd;
	};

	procStruct processes[256];

	/// <summary>
	/// Сводка для MyForm
	/// </summary>

	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}


	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  ProcessToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  startToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  pauseToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  FileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  stopToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  AboutprogramПрограмуToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  developerToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripButton^  start_toolStripButton;
	private: System::Windows::Forms::ToolStripButton^  pause_toolStripButton;
	private: System::Windows::Forms::ToolStripButton^  stop_toolStripButton;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
	private: System::Windows::Forms::ToolStripButton^  developer_toolStripButton;
	private: System::Windows::Forms::ToolStripButton^  help_toolStripButton;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripButton^  exit_toolStripButton;
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator6;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator7;
	private: System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog1;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::TabPage^  tabPage3;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown_timeUpdate;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown_history;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::ToolStripMenuItem^  зберигтиToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator4;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton1;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator5;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Імя;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ЦП;
	private: System::Windows::Forms::ToolStrip^  toolStrip1;


	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel2;

	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel3;
	private: System::Windows::Forms::ToolStripProgressBar^  toolStripProgressBar1;

	private: System::ComponentModel::IContainer^  components;

	private: UINT th32nameNpid()
	{
		HANDLE hProcessSnap;
		HANDLE hProcess;
		PROCESSENTRY32 pe32;
		DWORD dwPriorityClass; UINT procNum = 9;
		
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE)
			return procNum;
		
		pe32.dwSize = sizeof(PROCESSENTRY32);

		if (!Process32First(hProcessSnap, &pe32))
		{
			CloseHandle(hProcessSnap); return procNum;       
		}

		procNum = 0;
		do
		{
			processes[procNum].name = pe32.szExeFile;
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
			processes[procNum].pid = pe32.th32ProcessID;
			procNum++;
		} while (Process32Next(hProcessSnap, &pe32));
		CloseHandle(hProcessSnap);
		return procNum - 1;
	}

	//HANDLE hProcess,         // дескриптор процесса
	//LPFILETIME lpCreationTime, // время создания процесса
	//LPFILETIME lpExitTime,   // время выхода из работы процесса
	//LPFILETIME lpKernelTime, // время, работы процесса в режиме ядра
	//LPFILETIME lpUserTime    // время, работы процесса в режиме пользователя

	//hProcess

	// [in] Дескриптор процесса, информация о распределении интервалов времени которого разыскивается.Этот дескриптор должен быть создан с правами доступа PROCESS_QUERY_INFORMATION.Для получения дополнительной информации, см.статью Защита процесса и права доступа.

	// lpCreationTime

	// [out] Указатель на структуру FILETIME, которая принимает время создания процесса.

	// lpExitTime

	// [out] Указатель на структуру FILETIME, которая принимает время выхода из работы процесса.Если процесс не вышел из работы, содержание этой структуры не определенное.

	// lpKernelTime

	// [out] Указатель на структуру FILETIME, которая принимает величину времени, в течение которого процесс выполнялся в привилегированном режиме(режиме ядра).Чтобы получить это значение, определяется время, в ходе которого каждый из потоков процесса выполнялся в режиме ядра, а затем все эти периоды суммируются вместе.

	// lpUserTime

	// [out] Указатель на структуру FILETIME, которая принимает величину времени, в течение которого процесс выполнялся в непривилегированном(пользовательском) режиме.Чтобы получить это значение, определяется время, в ходе которого каждый из потоков процесса выполнялся в режиме ядра, а затем все эти периоды суммируются вместе.

	private: procStruct getProcessKtUt(int processID)
	{
		HANDLE hProcess; 
		FILETIME prCreatTime, prExitTime, prKrnlTime, prUserTime; procStruct tempProc;
		//флаг доступу, дискр наслыдування, id	
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, processID);

		if (GetProcessTimes(hProcess, &prCreatTime, &prExitTime, &prKrnlTime, &prUserTime))
		{
			tempProc.kt = prKrnlTime; tempProc.ut = prUserTime;
		}
		CloseHandle(hProcess);
		return tempProc;
	}

	private: procStruct getSysKtUtForProcess()
	{
		FILETIME ftSysIdle, ftSysKernel, ftSysUser; procStruct tempProc;
		if (GetSystemTimes(&ftSysIdle, &ftSysKernel, &ftSysUser))
		{
			tempProc.sit = ftSysIdle; tempProc.skt = ftSysKernel; tempProc.sut = ftSysUser;
		}
		return tempProc;
	}
	
	//різниця часів 
	private: ULONG64 getTimeDif(const FILETIME& oldTime, const FILETIME& newTime)
	{
		//структура
		LARGE_INTEGER young, old;
		young.LowPart = newTime.dwLowDateTime;
		young.HighPart = newTime.dwHighDateTime;

		old.LowPart = oldTime.dwLowDateTime;
		old.HighPart = oldTime.dwHighDateTime;

		return young.QuadPart - old.QuadPart;
	}


	private: void getProcesses()
	{
		UINT i;
		nOfReturnedIDs = th32nameNpid();
		for (i = 0; i < nOfReturnedIDs; i++)
		{
			processes[i].kt = getProcessKtUt(processes[i].pid).kt; processes[i].ut = getProcessKtUt(processes[i].pid).ut; // get kt,ut
			processes[i].skt = getSysKtUtForProcess().skt; processes[i].sut = getSysKtUtForProcess().sut;  // get skt,sut																				  
		}
	}


	private: int history_points = 60;
	private: void CalculateCPUandOutAll()
	{
		UINT i;
		real_time_processes.clear();
		for (i = 0; i < nOfReturnedIDs; i++)
		{
			processes[i].ktd = getTimeDif(processes[i].kt, getProcessKtUt(processes[i].pid).kt);
			processes[i].utd = getTimeDif(processes[i].ut, getProcessKtUt(processes[i].pid).ut);

			processes[i].skd = getTimeDif(processes[i].skt, getSysKtUtForProcess().skt);
			processes[i].sud = getTimeDif(processes[i].sut, getSysKtUtForProcess().sut);

			processes[i].ktutd = processes[i].ktd + processes[i].utd;
			processes[i].sktutd = processes[i].skd + processes[i].sud;
			processes[i].CPU = ((100.0 * processes[i].ktutd) / processes[i].sktutd);

			CPU_info pr(processes[i].pid, (processes[i].name).c_str(), processes[i].CPU);
			curent_add_process = pr;

			int points = 0;
			for (auto it = all_processes.begin(); it != all_processes.end(); ++it)
			{
				if (it->_id == pr._id)
				{
					++points;
				}
			}

			for (; points > history_points; --points)
			{
				auto it = remove_if(all_processes.begin(), all_processes.begin(), is_odd());
				all_processes.erase(it);
			}

			if (processes[i].CPU < 0 || processes[i].CPU >100) {
				pr._percent = 0;
				all_processes.push_back(pr);
				real_time_processes.push_back(pr);
			}
			else
			{
				all_processes.push_back(pr);
				real_time_processes.push_back(pr);
			}
		}
	}


	protected:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle3 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle4 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Title^  title1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator6 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->start_toolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->pause_toolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->stop_toolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->developer_toolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->help_toolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exit_toolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator7 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->FileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->зберигтиToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator5 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ProcessToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->startToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pauseToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->stopToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->AboutprogramПрограмуToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->developerToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->ID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Імя = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ЦП = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->folderBrowserDialog1 = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->numericUpDown_history = (gcnew System::Windows::Forms::NumericUpDown());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->numericUpDown_timeUpdate = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripProgressBar1 = (gcnew System::Windows::Forms::ToolStripProgressBar());
			this->toolStripStatusLabel2 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel3 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStrip1->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->tabPage3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown_history))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown_timeUpdate))->BeginInit();
			this->statusStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// toolStrip1
			// 
			this->toolStrip1->BackColor = System::Drawing::SystemColors::Menu;
			this->toolStrip1->GripMargin = System::Windows::Forms::Padding(0);
			this->toolStrip1->GripStyle = System::Windows::Forms::ToolStripGripStyle::Hidden;
			this->toolStrip1->ImageScalingSize = System::Drawing::Size(15, 15);
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(12) {
				this->toolStripSeparator4,
					this->toolStripButton1, this->toolStripSeparator6, this->start_toolStripButton, this->pause_toolStripButton, this->stop_toolStripButton,
					this->toolStripSeparator3, this->developer_toolStripButton, this->help_toolStripButton, this->toolStripSeparator2, this->exit_toolStripButton,
					this->toolStripSeparator7
			});
			this->toolStrip1->Location = System::Drawing::Point(0, 20);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Padding = System::Windows::Forms::Padding(0);
			this->toolStrip1->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			this->toolStrip1->Size = System::Drawing::Size(496, 25);
			this->toolStrip1->TabIndex = 0;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// toolStripSeparator4
			// 
			this->toolStripSeparator4->Name = L"toolStripSeparator4";
			this->toolStripSeparator4->Size = System::Drawing::Size(6, 25);
			// 
			// toolStripButton1
			// 
			this->toolStripButton1->AutoSize = false;
			this->toolStripButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton1.Image")));
			this->toolStripButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton1->Margin = System::Windows::Forms::Padding(0);
			this->toolStripButton1->Name = L"toolStripButton1";
			this->toolStripButton1->Size = System::Drawing::Size(24, 24);
			this->toolStripButton1->Text = L"Зберегти";
			this->toolStripButton1->Click += gcnew System::EventHandler(this, &MyForm::toolStripButton1_Click);
			// 
			// toolStripSeparator6
			// 
			this->toolStripSeparator6->Name = L"toolStripSeparator6";
			this->toolStripSeparator6->Size = System::Drawing::Size(6, 25);
			// 
			// start_toolStripButton
			// 
			this->start_toolStripButton->AutoSize = false;
			this->start_toolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->start_toolStripButton->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"start_toolStripButton.Image")));
			this->start_toolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->start_toolStripButton->Margin = System::Windows::Forms::Padding(0);
			this->start_toolStripButton->Name = L"start_toolStripButton";
			this->start_toolStripButton->Size = System::Drawing::Size(24, 24);
			this->start_toolStripButton->Text = L"Почати";
			this->start_toolStripButton->Click += gcnew System::EventHandler(this, &MyForm::start_toolStripButton_Click);
			// 
			// pause_toolStripButton
			// 
			this->pause_toolStripButton->AutoSize = false;
			this->pause_toolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->pause_toolStripButton->Enabled = false;
			this->pause_toolStripButton->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pause_toolStripButton.Image")));
			this->pause_toolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->pause_toolStripButton->Margin = System::Windows::Forms::Padding(0);
			this->pause_toolStripButton->Name = L"pause_toolStripButton";
			this->pause_toolStripButton->Size = System::Drawing::Size(24, 24);
			this->pause_toolStripButton->Text = L"Зупинити";
			this->pause_toolStripButton->Click += gcnew System::EventHandler(this, &MyForm::pause_toolStripButton_Click);
			// 
			// stop_toolStripButton
			// 
			this->stop_toolStripButton->AutoSize = false;
			this->stop_toolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->stop_toolStripButton->Enabled = false;
			this->stop_toolStripButton->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"stop_toolStripButton.Image")));
			this->stop_toolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->stop_toolStripButton->Margin = System::Windows::Forms::Padding(0);
			this->stop_toolStripButton->Name = L"stop_toolStripButton";
			this->stop_toolStripButton->Size = System::Drawing::Size(24, 24);
			this->stop_toolStripButton->Text = L"Завершити";
			this->stop_toolStripButton->Click += gcnew System::EventHandler(this, &MyForm::stop_toolStripButton_Click);
			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			this->toolStripSeparator3->Size = System::Drawing::Size(6, 25);
			// 
			// developer_toolStripButton
			// 
			this->developer_toolStripButton->AutoSize = false;
			this->developer_toolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->developer_toolStripButton->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"developer_toolStripButton.Image")));
			this->developer_toolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->developer_toolStripButton->Margin = System::Windows::Forms::Padding(0);
			this->developer_toolStripButton->Name = L"developer_toolStripButton";
			this->developer_toolStripButton->Size = System::Drawing::Size(24, 24);
			this->developer_toolStripButton->Text = L"Про програму";
			this->developer_toolStripButton->Click += gcnew System::EventHandler(this, &MyForm::developer_toolStripButton_Click);
			// 
			// help_toolStripButton
			// 
			this->help_toolStripButton->AutoSize = false;
			this->help_toolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->help_toolStripButton->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"help_toolStripButton.Image")));
			this->help_toolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->help_toolStripButton->Margin = System::Windows::Forms::Padding(0);
			this->help_toolStripButton->Name = L"help_toolStripButton";
			this->help_toolStripButton->Size = System::Drawing::Size(24, 24);
			this->help_toolStripButton->Text = L"Переглянути довідку";
			this->help_toolStripButton->Click += gcnew System::EventHandler(this, &MyForm::help_toolStripButton_Click);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(6, 25);
			// 
			// exit_toolStripButton
			// 
			this->exit_toolStripButton->AutoSize = false;
			this->exit_toolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->exit_toolStripButton->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"exit_toolStripButton.Image")));
			this->exit_toolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->exit_toolStripButton->Margin = System::Windows::Forms::Padding(0);
			this->exit_toolStripButton->Name = L"exit_toolStripButton";
			this->exit_toolStripButton->Size = System::Drawing::Size(24, 24);
			this->exit_toolStripButton->Text = L"Вихiд";
			this->exit_toolStripButton->Click += gcnew System::EventHandler(this, &MyForm::exit_toolStripButton_Click);
			// 
			// toolStripSeparator7
			// 
			this->toolStripSeparator7->Name = L"toolStripSeparator7";
			this->toolStripSeparator7->Size = System::Drawing::Size(6, 25);
			// 
			// menuStrip1
			// 
			this->menuStrip1->AutoSize = false;
			this->menuStrip1->BackColor = System::Drawing::SystemColors::Menu;
			this->menuStrip1->GripMargin = System::Windows::Forms::Padding(0);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->FileToolStripMenuItem,
					this->ProcessToolStripMenuItem, this->AboutprogramПрограмуToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(1);
			this->menuStrip1->Size = System::Drawing::Size(496, 20);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// FileToolStripMenuItem
			// 
			this->FileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->зберигтиToolStripMenuItem,
					this->toolStripSeparator5, this->exitToolStripMenuItem
			});
			this->FileToolStripMenuItem->Name = L"FileToolStripMenuItem";
			this->FileToolStripMenuItem->Size = System::Drawing::Size(48, 18);
			this->FileToolStripMenuItem->Text = L"&Файл";
			// 
			// зберигтиToolStripMenuItem
			// 
			this->зберигтиToolStripMenuItem->Name = L"зберигтиToolStripMenuItem";
			this->зберигтиToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::S));
			this->зберигтиToolStripMenuItem->Size = System::Drawing::Size(164, 22);
			this->зберигтиToolStripMenuItem->Text = L"Зберегти";
			this->зберигтиToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::зберигтиToolStripMenuItem_Click);
			// 
			// toolStripSeparator5
			// 
			this->toolStripSeparator5->Name = L"toolStripSeparator5";
			this->toolStripSeparator5->Size = System::Drawing::Size(161, 6);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F12;
			this->exitToolStripMenuItem->Size = System::Drawing::Size(164, 22);
			this->exitToolStripMenuItem->Text = L"Вихiд";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::exitToolStripMenuItem_Click);
			// 
			// ProcessToolStripMenuItem
			// 
			this->ProcessToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->startToolStripMenuItem,
					this->pauseToolStripMenuItem, this->toolStripSeparator1, this->stopToolStripMenuItem
			});
			this->ProcessToolStripMenuItem->Name = L"ProcessToolStripMenuItem";
			this->ProcessToolStripMenuItem->Size = System::Drawing::Size(61, 18);
			this->ProcessToolStripMenuItem->Text = L"&Процес";
			// 
			// startToolStripMenuItem
			// 
			this->startToolStripMenuItem->Name = L"startToolStripMenuItem";
			this->startToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F5;
			this->startToolStripMenuItem->Size = System::Drawing::Size(155, 22);
			this->startToolStripMenuItem->Text = L"Почати";
			this->startToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::startToolStripMenuItem_Click);
			// 
			// pauseToolStripMenuItem
			// 
			this->pauseToolStripMenuItem->Enabled = false;
			this->pauseToolStripMenuItem->Name = L"pauseToolStripMenuItem";
			this->pauseToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F6;
			this->pauseToolStripMenuItem->Size = System::Drawing::Size(155, 22);
			this->pauseToolStripMenuItem->Text = L"Зупинити";
			this->pauseToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::pauseToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(152, 6);
			// 
			// stopToolStripMenuItem
			// 
			this->stopToolStripMenuItem->Enabled = false;
			this->stopToolStripMenuItem->Name = L"stopToolStripMenuItem";
			this->stopToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F7;
			this->stopToolStripMenuItem->Size = System::Drawing::Size(155, 22);
			this->stopToolStripMenuItem->Text = L"Завершити";
			this->stopToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::stopToolStripMenuItem_Click);
			// 
			// AboutprogramПрограмуToolStripMenuItem
			// 
			this->AboutprogramПрограмуToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->developerToolStripMenuItem,
					this->helpToolStripMenuItem
			});
			this->AboutprogramПрограмуToolStripMenuItem->Name = L"AboutprogramПрограмуToolStripMenuItem";
			this->AboutprogramПрограмуToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F1;
			this->AboutprogramПрограмуToolStripMenuItem->Size = System::Drawing::Size(61, 18);
			this->AboutprogramПрограмуToolStripMenuItem->Text = L"&Довідка";
			// 
			// developerToolStripMenuItem
			// 
			this->developerToolStripMenuItem->Name = L"developerToolStripMenuItem";
			this->developerToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>(((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Shift)
				| System::Windows::Forms::Keys::I));
			this->developerToolStripMenuItem->Size = System::Drawing::Size(223, 22);
			this->developerToolStripMenuItem->Text = L"Про програму";
			this->developerToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::developerToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F1;
			this->helpToolStripMenuItem->Size = System::Drawing::Size(223, 22);
			this->helpToolStripMenuItem->Text = L"Переглянути довідку";
			this->helpToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::helpToolStripMenuItem_Click);
			// 
			// dataGridView1
			// 
			this->dataGridView1->AccessibleRole = System::Windows::Forms::AccessibleRole::None;
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->AllowUserToOrderColumns = true;
			this->dataGridView1->AllowUserToResizeRows = false;
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
			this->dataGridView1->BackgroundColor = System::Drawing::SystemColors::Window;
			this->dataGridView1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->dataGridView1->CellBorderStyle = System::Windows::Forms::DataGridViewCellBorderStyle::None;
			this->dataGridView1->ClipboardCopyMode = System::Windows::Forms::DataGridViewClipboardCopyMode::Disable;
			this->dataGridView1->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {
				this->ID, this->Імя,
					this->ЦП
			});
			dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			dataGridViewCellStyle2->BackColor = System::Drawing::SystemColors::Window;
			dataGridViewCellStyle2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			dataGridViewCellStyle2->ForeColor = System::Drawing::SystemColors::ControlText;
			dataGridViewCellStyle2->SelectionBackColor = System::Drawing::SystemColors::GradientInactiveCaption;
			dataGridViewCellStyle2->SelectionForeColor = System::Drawing::SystemColors::Desktop;
			dataGridViewCellStyle2->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->dataGridView1->DefaultCellStyle = dataGridViewCellStyle2;
			this->dataGridView1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dataGridView1->GridColor = System::Drawing::SystemColors::Window;
			this->dataGridView1->Location = System::Drawing::Point(0, 0);
			this->dataGridView1->MultiSelect = false;
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Sunken;
			dataGridViewCellStyle3->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			dataGridViewCellStyle3->BackColor = System::Drawing::SystemColors::Control;
			dataGridViewCellStyle3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			dataGridViewCellStyle3->ForeColor = System::Drawing::SystemColors::WindowText;
			dataGridViewCellStyle3->SelectionBackColor = System::Drawing::SystemColors::Highlight;
			dataGridViewCellStyle3->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle3->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->dataGridView1->RowHeadersDefaultCellStyle = dataGridViewCellStyle3;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowHeadersWidth = 21;
			dataGridViewCellStyle4->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			this->dataGridView1->RowsDefaultCellStyle = dataGridViewCellStyle4;
			this->dataGridView1->RowTemplate->Height = 18;
			this->dataGridView1->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->Size = System::Drawing::Size(490, 249);
			this->dataGridView1->TabIndex = 5;
			this->dataGridView1->CellMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &MyForm::dataGridView1_CellMouseClick);
			this->dataGridView1->ColumnHeaderMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &MyForm::dataGridView1_ColumnHeaderMouseClick);
			// 
			// ID
			// 
			this->ID->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			dataGridViewCellStyle1->Format = L"N0";
			dataGridViewCellStyle1->NullValue = nullptr;
			this->ID->DefaultCellStyle = dataGridViewCellStyle1;
			this->ID->FillWeight = 50;
			this->ID->HeaderText = L"ID";
			this->ID->MaxInputLength = 1000;
			this->ID->Name = L"ID";
			this->ID->ReadOnly = true;
			// 
			// Імя
			// 
			this->Імя->HeaderText = L"Ім\'я ";
			this->Імя->MaxInputLength = 1000;
			this->Імя->Name = L"Імя";
			this->Імя->ReadOnly = true;
			// 
			// ЦП
			// 
			this->ЦП->FillWeight = 50;
			this->ЦП->HeaderText = L"ЦП";
			this->ЦП->MaxInputLength = 1000;
			this->ЦП->Name = L"ЦП";
			this->ЦП->ReadOnly = true;
			// 
			// chart1
			// 
			this->chart1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			chartArea1->AxisX->ScrollBar->BackColor = System::Drawing::SystemColors::ActiveBorder;
			chartArea1->AxisX->ScrollBar->ButtonColor = System::Drawing::SystemColors::ActiveBorder;
			chartArea1->AxisX->ScrollBar->LineColor = System::Drawing::SystemColors::ActiveCaptionText;
			chartArea1->BackColor = System::Drawing::SystemColors::Window;
			chartArea1->BackSecondaryColor = System::Drawing::SystemColors::Window;
			chartArea1->Name = L"ChartArea1";
			chartArea1->Position->Auto = false;
			chartArea1->Position->Height = 90;
			chartArea1->Position->Width = 96;
			chartArea1->Position->Y = 10;
			this->chart1->ChartAreas->Add(chartArea1);
			legend1->Enabled = false;
			legend1->Name = L"Legend1";
			this->chart1->Legends->Add(legend1);
			this->chart1->Location = System::Drawing::Point(0, 30);
			this->chart1->Name = L"chart1";
			series1->BorderWidth = 2;
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series1->IsVisibleInLegend = false;
			series1->Legend = L"Legend1";
			series1->Name = L"Series1";
			this->chart1->Series->Add(series1);
			this->chart1->Size = System::Drawing::Size(498, 230);
			this->chart1->TabIndex = 6;
			this->chart1->Text = L"chart1";
			title1->Alignment = System::Drawing::ContentAlignment::MiddleRight;
			title1->Docking = System::Windows::Forms::DataVisualization::Charting::Docking::Left;
			title1->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei UI", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			title1->Name = L"Title1";
			this->chart1->Titles->Add(title1);
			// 
			// tabControl1
			// 
			this->tabControl1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Controls->Add(this->tabPage3);
			this->tabControl1->Location = System::Drawing::Point(0, 45);
			this->tabControl1->Margin = System::Windows::Forms::Padding(0);
			this->tabControl1->Multiline = true;
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(498, 275);
			this->tabControl1->TabIndex = 7;
			this->tabControl1->Click += gcnew System::EventHandler(this, &MyForm::tabControl1_Click);
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->dataGridView1);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Margin = System::Windows::Forms::Padding(0);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Size = System::Drawing::Size(490, 249);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Процеси";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->chart1);
			this->tabPage2->Controls->Add(this->comboBox1);
			this->tabPage2->Controls->Add(this->label1);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Size = System::Drawing::Size(498, 260);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Хронологія";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// comboBox1
			// 
			this->comboBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->comboBox1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox1->Enabled = false;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(48, 3);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(447, 21);
			this->comboBox1->TabIndex = 8;
			this->comboBox1->SelectionChangeCommitted += gcnew System::EventHandler(this, &MyForm::comboBox1_SelectionChangeCommitted);
			this->comboBox1->Click += gcnew System::EventHandler(this, &MyForm::comboBox1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(3, 6);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(51, 13);
			this->label1->TabIndex = 7;
			this->label1->Text = L"Процес: ";
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->numericUpDown_history);
			this->tabPage3->Controls->Add(this->label3);
			this->tabPage3->Controls->Add(this->button1);
			this->tabPage3->Controls->Add(this->numericUpDown_timeUpdate);
			this->tabPage3->Controls->Add(this->label2);
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Size = System::Drawing::Size(498, 260);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"Налаштування";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// numericUpDown_history
			// 
			this->numericUpDown_history->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->numericUpDown_history->Location = System::Drawing::Point(370, 6);
			this->numericUpDown_history->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 500, 0, 0, 0 });
			this->numericUpDown_history->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->numericUpDown_history->Name = L"numericUpDown_history";
			this->numericUpDown_history->Size = System::Drawing::Size(60, 20);
			this->numericUpDown_history->TabIndex = 8;
			this->numericUpDown_history->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 60, 0, 0, 0 });
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(3, 6);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(361, 13);
			this->label3->TabIndex = 7;
			this->label3->Text = L"Історія (кількість збережених показників завантаженості процесора):";
			// 
			// button1
			// 
			this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->button1->Location = System::Drawing::Point(418, 228);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 26);
			this->button1->TabIndex = 6;
			this->button1->Text = L"Змінити";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// numericUpDown_timeUpdate
			// 
			this->numericUpDown_timeUpdate->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->numericUpDown_timeUpdate->DecimalPlaces = 1;
			this->numericUpDown_timeUpdate->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 65536 });
			this->numericUpDown_timeUpdate->Location = System::Drawing::Point(370, 32);
			this->numericUpDown_timeUpdate->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 0 });
			this->numericUpDown_timeUpdate->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 65536 });
			this->numericUpDown_timeUpdate->Name = L"numericUpDown_timeUpdate";
			this->numericUpDown_timeUpdate->Size = System::Drawing::Size(60, 20);
			this->numericUpDown_timeUpdate->TabIndex = 5;
			this->numericUpDown_timeUpdate->ThousandsSeparator = true;
			this->numericUpDown_timeUpdate->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(227, 32);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(137, 13);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Час оновлення даних (с.):";
			// 
			// statusStrip1
			// 
			this->statusStrip1->AutoSize = false;
			this->statusStrip1->BackColor = System::Drawing::SystemColors::Menu;
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->toolStripProgressBar1,
					this->toolStripStatusLabel2, this->toolStripStatusLabel3
			});
			this->statusStrip1->Location = System::Drawing::Point(0, 320);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->RenderMode = System::Windows::Forms::ToolStripRenderMode::ManagerRenderMode;
			this->statusStrip1->Size = System::Drawing::Size(496, 20);
			this->statusStrip1->TabIndex = 8;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripProgressBar1
			// 
			this->toolStripProgressBar1->MarqueeAnimationSpeed = 5;
			this->toolStripProgressBar1->Name = L"toolStripProgressBar1";
			this->toolStripProgressBar1->Size = System::Drawing::Size(100, 14);
			this->toolStripProgressBar1->Step = 1;
			this->toolStripProgressBar1->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			// 
			// toolStripStatusLabel2
			// 
			this->toolStripStatusLabel2->Name = L"toolStripStatusLabel2";
			this->toolStripStatusLabel2->Size = System::Drawing::Size(13, 15);
			this->toolStripStatusLabel2->Text = L"  ";
			// 
			// toolStripStatusLabel3
			// 
			this->toolStripStatusLabel3->Name = L"toolStripStatusLabel3";
			this->toolStripStatusLabel3->Size = System::Drawing::Size(13, 15);
			this->toolStripStatusLabel3->Text = L"  ";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(96, 96);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Dpi;
			this->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->ClientSize = System::Drawing::Size(496, 340);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->menuStrip1);
			this->DoubleBuffered = true;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menuStrip1;
			this->MinimumSize = System::Drawing::Size(512, 378);
			this->Name = L"MyForm";
			this->Text = L"CPU loading";
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			this->tabPage3->ResumeLayout(false);
			this->tabPage3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown_history))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown_timeUpdate))->EndInit();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	//------------------------------------------------------------------------------------------------------------------------------
	Int32 sort_colums = 0;
	Int32 select_rows = 0;
	String^ select_id;
	UINT number_pr;
	private: void Set_DataGrid()
	{
		if (number_pr>0)
		{
			select_id = dataGridView1[0, dataGridView1->CurrentRow->Index]->Value->ToString();
		}
		Int32 scrolBarPosition = dataGridView1->FirstDisplayedScrollingRowIndex;
		dataGridView1->Rows->Clear();
		CalculateCPUandOutAll();

		number_pr = 0;
		int CPU;
		for(auto it= real_time_processes.begin(); it!=real_time_processes.end(); ++it)
		{
			++number_pr;
			dataGridView1->Rows->Add(it->_id, gcnew String(it->_name.c_str()), Math::Round(it->_percent));
			CPU += Math::Round(it->_percent);
		}
		toolStripStatusLabel2->Text = "Процесів: " + number_pr;
		if (CPU <= 100) { toolStripStatusLabel3->Text = "Завантаження ЦП: " + CPU +"%"; }
		else if(CPU > 100) { toolStripStatusLabel3->Text = "Завантаження ЦП: " + "100" + "%"; }
		else { toolStripStatusLabel3->Text = "Завантаження ЦП: " + "0" + "%"; }

		if (number_pr > 0)
		{
			if (select_rows >= 0 && select_rows<number_pr)
				dataGridView1->Rows[select_rows]->Selected = true;

			for (int row = 0; row < real_time_processes.size(); ++row)
			{
				if (select_id == dataGridView1[0, row]->Value->ToString())
				{
					dataGridView1->Rows[row]->Selected = true;
					dataGridView1->CurrentCell = dataGridView1[0, row];	
				}
			}

			if (scrolBarPosition <= 0) 
				dataGridView1->FirstDisplayedScrollingRowIndex = 0;
			else if (scrolBarPosition >= number_pr)
				dataGridView1->FirstDisplayedScrollingRowIndex = number_pr - 1;
			else
				dataGridView1->FirstDisplayedScrollingRowIndex = scrolBarPosition;

			if (dataGridView1->SortOrder == SortOrder::Ascending)
				dataGridView1->Sort(dataGridView1->Columns[sort_colums], ListSortDirection::Ascending);
			if (dataGridView1->SortOrder == SortOrder::Descending)
				dataGridView1->Sort(dataGridView1->Columns[sort_colums], ListSortDirection::Descending);
		}
	}
	private: System::Void dataGridView1_ColumnHeaderMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
		sort_colums = e->ColumnIndex;
	}
	private: System::Void dataGridView1_CellMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
		select_rows = e->RowIndex;
	}
	private: System::Void tabControl1_Click(System::Object^  sender, System::EventArgs^  e) {
		dataGridView1->Focus();
	}
	//------------------------------------------------------------------------------------------------------------------------------
	private: void Set_Chart() {

		while (chart1->Titles->Count > 0)
		{
			chart1->Titles->Clear();
		}
		while (chart1->Series["Series1"]->Points->Count > 0)
		{
			chart1->Series["Series1"]->Points->Clear();
		}

		chart1->ChartAreas[0]->AxisX->ScaleView->Size = 10;
		int time = 0;
		String^ str_name;
		String^ name_Title;
		for (auto it = all_processes.begin(); it != all_processes.end(); ++it)
		{		
			str_name = gcnew System::String(it->_name.c_str());
			String^ str_id = gcnew System::String(it->_id.ToString());
					
			if (str_id == dataGridView1[0, dataGridView1->CurrentRow->Index]->Value->ToString())
			{
				chart1->Series["Series1"]->Points->AddXY(time++, it->_percent);

				if (time > 10)
					chart1->ChartAreas[0]->AxisX->ScaleView->Position = time-10;	

				name_Title = str_name + "—" + str_id;
			}
		}
		if (name_Title != nullptr)
		{
			chart1->Titles->Add(name_Title);
		}
	};
	//------------------------------------------------------------------------------------------------------------------------------
	double update_time = 1;

	private: void ProgressZero()
	{
		toolStripProgressBar1->Value = 0;
	}
	private: void ProgressBar_Set()
	{
		if (toolStripProgressBar1->Value < 100)
		{
			toolStripProgressBar1->Value+=10;
		}
	}

	private: void Set_Data()
	{	
		while (true)
		{
			if (this->InvokeRequired)
				this->Invoke(gcnew Action(this, &MyForm::getProcesses));
			else
				getProcesses();

			if (this->InvokeRequired)
				this->Invoke(gcnew Action(this, &MyForm::ProgressZero));
			else
				ProgressZero();

			for (int tup = 0; tup < 10; ++tup)
			{	
				if (this->InvokeRequired)
					this->Invoke(gcnew Action(this, &MyForm::ProgressBar_Set));
				else
					ProgressBar_Set();
				WriteTimeThread->Sleep(((update_time * 1000) / 100) * 10);
			}

			if (this->InvokeRequired)
				this->Invoke(gcnew Action(this, &MyForm::Set_DataGrid));
			else
				Set_DataGrid();

			comboBox1->Enabled = true;

			if (this->InvokeRequired)
				this->Invoke(gcnew Action(this, &MyForm::Set_Chart));
			else
				Set_Chart();
		}
	};	
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		Clear_All();

		numericUpDown_timeUpdate->Value = System::Convert::ToDecimal(numericUpDown_timeUpdate->Value.ToString());
		update_time = System::Convert::ToDouble(numericUpDown_timeUpdate->Value.ToString());

		numericUpDown_history->Value = System::Convert::ToDecimal(numericUpDown_history->Value.ToString());
		history_points = System::Convert::ToDouble(numericUpDown_history->Value.ToString());
	}
	//------------------------------------------------------------------------------------------------------------------------------
	private: void Save_Data()
	{
		Stream^ my_Stream;
		SaveFileDialog^ saveFileDialog1 = gcnew SaveFileDialog;
		saveFileDialog1->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
		saveFileDialog1->FilterIndex = 1;
		saveFileDialog1->RestoreDirectory = true;

		String^ data_output;
		if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			if ((my_Stream = saveFileDialog1->OpenFile()) != nullptr)
			{
				StreamWriter^ sw = gcnew StreamWriter(my_Stream);
				sw->WriteLine("Дата і час створення: " + DateTime::Now);

				real_time_processes.sort();
				for (auto it = real_time_processes.begin(); it != real_time_processes.end(); ++it)
				{
					String^ save_id = gcnew System::String(it->_id.ToString());
					String^ save_name = gcnew System::String(it->_name.c_str());
					String^ save_percent = gcnew System::String(it->_percent.ToString());

					sw->WriteLine("ID: " + save_id + "; "
						+ "Ім'я: " + save_name + "; "
						+ "ЦП: " + save_percent);
					dataGridView1->Rows->Add(it->_id, gcnew String(it->_name.c_str()), it->_percent);
				}

				sw->Close();
				my_Stream->Close();
			}
		}
	}
	private: System::Void зберигтиToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) { Save_Data();}
	private: System::Void toolStripButton1_Click(System::Object^  sender, System::EventArgs^  e) { Save_Data(); }
	//------------------------------------------------------------------------------------------------------------------------------
	private:  System::Threading::Thread^ WriteTimeThread;
	private: void Start()
	{
		dataGridView1->Focus();
		startToolStripMenuItem->Enabled = false;
		pauseToolStripMenuItem->Enabled = true;
		stopToolStripMenuItem->Enabled = true;
		start_toolStripButton->Enabled = false;
		pause_toolStripButton->Enabled = true;
		stop_toolStripButton->Enabled = true;

	
		WriteTimeThread = gcnew Thread(gcnew ThreadStart(this, &MyForm::Set_Data));
		WriteTimeThread->IsBackground = true;
		WriteTimeThread->Priority = ThreadPriority::Highest;
		WriteTimeThread->Start();
	};
	private: System::Void startToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {Start();}
	private: System::Void start_toolStripButton_Click(System::Object^  sender, System::EventArgs^  e) {Start();}
	//------------------------------------------------------------------------------------------------------------------------------
	private: void Pause()
	{
		toolStripProgressBar1->Value = 0;

		startToolStripMenuItem->Enabled = true;
		pauseToolStripMenuItem->Enabled = false;
		stop_toolStripButton->Enabled = true;
		start_toolStripButton->Enabled = true;
		pause_toolStripButton->Enabled = false;
		stop_toolStripButton->Enabled = true;
		
		comboBox1->Enabled = false;

		comboBox1->Items->Clear();
		comboBox1->Text = "";

		WriteTimeThread->Suspend();
	};
	private: System::Void pauseToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {Pause();}
	private: System::Void pause_toolStripButton_Click(System::Object^  sender, System::EventArgs^  e) {Pause();}
	//------------------------------------------------------------------------------------------------------------------------------	
	private: void Clear_All()
	{
		while (chart1->Titles->Count > 0)
		{
			chart1->Titles->Clear();
		}
		while (chart1->Series["Series1"]->Points->Count > 0)
		{
			chart1->Series["Series1"]->Points->Clear();
		}
		comboBox1->Items->Clear();
		comboBox1->Text = "";

		all_processes.clear();
		real_time_processes.clear();
		
		chart1->ChartAreas[0]->AxisX->ScaleView->Position = 0;
	}
	private: void Stop()
	{
		toolStripProgressBar1->Value = 0;

		startToolStripMenuItem->Enabled = true;
		pauseToolStripMenuItem->Enabled = false;
		stopToolStripMenuItem->Enabled = false;
		start_toolStripButton->Enabled = true;
		pause_toolStripButton->Enabled = false;
		stop_toolStripButton->Enabled = false;

		Clear_All();

		dataGridView1->Rows->Clear();
		comboBox1->Enabled = false;
		WriteTimeThread->Suspend();

		toolStripStatusLabel2->Text = " ";
		toolStripStatusLabel3->Text = " ";
		
		select_id="";
		number_pr =0;
	}
	private: System::Void stopToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {Stop();}
	private: System::Void stop_toolStripButton_Click(System::Object^  sender, System::EventArgs^  e) {Stop();}
	//------------------------------------------------------------------------------------------------------------------------------
	void AboutProgram_W_Start()
	{
		auto form = gcnew AboutProgram();
		form->ShowDialog();
	}
	private: System::Void developerToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {AboutProgram_W_Start();}
	private: System::Void developer_toolStripButton_Click(System::Object^  sender, System::EventArgs^  e) {AboutProgram_W_Start();}
	//------------------------------------------------------------------------------------------------------------------------------
	void Help_W_Start()
	{
		ShellExecute(NULL, _T("open"), _T("Довідка.pdf"), NULL, NULL, SW_SHOW);
	}
	private: System::Void help_toolStripButton_Click(System::Object^  sender, System::EventArgs^  e) {Help_W_Start();}
	private: System::Void helpToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {Help_W_Start();}
	//------------------------------------------------------------------------------------------------------------------------------
	private: System::Void comboBox1_Click(System::Object^  sender, System::EventArgs^  e) {
		comboBox1->Items->Clear();
		real_time_processes.sort();
		for (auto it = real_time_processes.begin(); it != real_time_processes.end(); ++it)
		{
			String^ str_name = gcnew System::String(it->_name.c_str());
			comboBox1->Items->Add(str_name+"—"+it->_id);
		}	
	}
	private: System::Void comboBox1_SelectionChangeCommitted(System::Object^  sender, System::EventArgs^  e) {
		for (int row = 0; row < number_pr; ++row)
		{
			if (comboBox1->Text == (dataGridView1[1, row]->Value->ToString() + "—" + dataGridView1[0, row]->Value->ToString()))
			{
				dataGridView1->Rows[row]->Selected = true;
				dataGridView1->CurrentCell = dataGridView1[0, row];
			}
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------
	private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) { Application::Exit(); }
	private: System::Void exit_toolStripButton_Click(System::Object^  sender, System::EventArgs^  e) { Application::Exit(); }
	//------------------------------------------------------------------------------------------------------------------------------
};
}
