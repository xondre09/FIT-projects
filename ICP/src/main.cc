/*!
 * \file    main.cc
 * \authors Karel ONDŘEJ, FIT
 * \authors David SMÉKAL, FIT
 * \date    7. 5. 2017
 * \brief   Vykreslení dlaždic.
 */

#include "main.h"

/*
 * Pridani menu pro počet her.
 */
Main::Main()
    : games(0)
{

    QMenuBar *menuBar = new QMenuBar();
    this->addWidget(menuBar, 0, 0, 1, 2);
    QMenu *menu = menuBar->addMenu("Games");
    QAction *action1 = menu->addAction("1 game");
    QAction *action2 = menu->addAction("2 games");
    QAction *action3 = menu->addAction("3 games");
    QAction *action4 = menu->addAction("4 games");

    connect(action1, &QAction::triggered, this, [this]{ grid(1);});
    connect(action2, &QAction::triggered, this, [this]{ grid(2);});
    connect(action3, &QAction::triggered, this, [this]{ grid(3);});
    connect(action4, &QAction::triggered, this, [this]{ grid(4);});

    this->grid(1);
}

/*
 * Vykresleni dlaždic
 */
void Main::grid(int numberOfGames)
{
    for(int i = numberOfGames; i < 4; ++i)
    {
        QLayoutItem *item = this->itemAtPosition(1 + i/2, i%2);
        if(item != 0)
        {
            this->removeItem(item);
            delete item->widget();
        }
        if(numberOfGames == 1)
        {
            this->setRowStretch(1 + i/2, 0);
            this->setColumnStretch(i % 2, 0);
        }
    }

    for(int i = games; i < 4; i++)
    {
        if( i < numberOfGames)
        {
            SolitaireWindow *widget = new SolitaireWindow();
            this->addWidget(widget, 1 + i/2, i%2);          
        }
        else if (numberOfGames == 1)
        {
            continue;
        }
        else
        {
            QWidget *widget = new QWidget();
            this->addWidget(widget, 1 + i/2, i%2);
        }

        this->setRowStretch(1 + i/2, 1);
        this->setColumnStretch(i % 2, 1);
    }
    this->update();
    games = numberOfGames;
}

/*!
 * \brief Vykreslení hry.
 */
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    Main *gl = new Main();

    QWidget window;
    window.setLayout(gl);
    window.show();

    return a.exec();
}
