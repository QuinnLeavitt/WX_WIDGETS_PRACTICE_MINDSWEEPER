// Quinn Leavitt
// WXWidgets Practice Minesweeper Project

#include "cMain.h"

//assigns the button to the buttonclicked function for the button event
wxBEGIN_EVENT_TABLE(cMain, wxFrame)
wxEND_EVENT_TABLE()


cMain::cMain() : wxFrame(nullptr, wxID_ANY, "WX Widgets Minesweeper Practice", wxPoint(30, 30), wxSize(800, 600))
{
	
	btn = new wxButton * [nFieldWidth * nFieldHeight];
	wxGridSizer* grid = new wxGridSizer(nFieldWidth, nFieldHeight, 0, 0);

	nField = new int[nFieldWidth * nFieldHeight];

	wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	for (int x = 0; x < nFieldWidth; x++)
	{
		for (int y = 0; y < nFieldHeight; y++)
		{
			btn[y * nFieldWidth + x] = new wxButton(this, 10000 + (y * nFieldWidth + x));
			btn[y * nFieldWidth + x]->SetFont(font);
			grid->Add(btn[y * nFieldWidth + x], 1, wxEXPAND | wxALL);

			btn[y * nFieldWidth + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);
			nField[y * nFieldWidth + x] = 0;
		}
	}

	this->SetSizer(grid);
	grid->Layout();

}


cMain::~cMain()
{
	delete[]btn;
	delete[]nField;
}

void cMain::OnButtonClicked(wxCommandEvent& evt)
{
	//m_list1->AppendString(m_txt1->GetValue()); Adds text to the listbox
	int x = (evt.GetId() - 10000) % nFieldWidth;
	int y = (evt.GetId() - 10000) / nFieldWidth;


	if (bFirstClick) // Populates minefield on first click
	{
		int mines = 30;
		while (mines)
		{
			int rx = rand() % nFieldWidth;
			int ry = rand() % nFieldHeight;

			if (nField[ry * nFieldWidth + rx] == 0 && rx != x && ry != y)
			{
				nField[ry * nFieldWidth + rx] = -1;
				mines--;
			}
 		}
		bFirstClick = false;
	}

	// Disable Button preventing it from being pressed again
	btn[y * nFieldWidth + x]->Enable(false);

	// Check if player hit a mine
	if (nField[y * nFieldWidth + x] == -1)
	{
		wxMessageBox("KABOOOOOM ! ! ! - Game Over >:D");
		// Reset Game
		bFirstClick = true;
		for (int x = 0; x < nFieldWidth; x++)
			for (int y = 0; y < nFieldHeight; y++)
			{
				nField[y * nFieldWidth + x] = 0;
				btn[y * nFieldWidth + x]->SetLabel("");
				btn[y * nFieldWidth + x]->Enable(true);
			}

	}
	else
	{
		// Count neighboring mines
		int mine_count = 0;
			for(int i = -1; i < 2; i++)
				for (int j = -1; j < 2; j++)
				{
					if (x + i >= 0 && x + i < nFieldWidth && y + j >= 0 && y + j < nFieldHeight)
					{
						if (nField[(y + j) * nFieldWidth + (x + i)] == -1)
							mine_count++;
					}
				}
		// Update button label to show mine count if > 0
			if (mine_count > 0)
			{
				btn[y * nFieldWidth + x]->SetLabel(std::to_string(mine_count));
			}
	}

	evt.Skip(); // Lets the macro know the event is finished and it can skip checking the parent containers
}