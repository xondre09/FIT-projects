/*!
* \file    solitairegui.cc
* \authors Karel ONDŘEJ, FIT
* \authors David SMÉKAL, FIT
* \date    5. 5. 2017
* \brief   Plátno na které se vykreslují karty.
*/
#include "solitairegui.h"

class SolitaireWindow;

/*
 * Vykresleni podkladu platna.
 */
SolitaireGui::SolitaireGui(Game* game_, SolitaireWindow *parent)
    : QGraphicsScene(parent),
      game(game_),
      active(NULL),
      victory(NULL)
{
    QIcon icon;
    hints[0] = NULL;
    hints[1] = NULL;
    // vykresleni karty konce balíku
    icon = QIcon(Solitaire::LAST_CARD);
    flipDeck = new CardGui(icon, this);
    flipDeck->draw();
    flipDeck->setZValue(-1);
    // Vykresleni podkladových karet
    icon = QIcon(Solitaire::EMPTY_CARD);
    for(int i= 0; i < 12; ++i) {
        CardGui *l =  new CardGui(icon, this);
        backCards.push_back(l);
        l->setPosition(i, 0);
        l->setMoveable(0);
        l->setZValue(-0.5);
        l->draw();
    }

    QFont font;
    font.setPixelSize(SCORE_SIZE);
    // umístění textu skore
    score = new QGraphicsTextItem();
    score->setFont(font);
    score->setPos(DECK_OFFSET_X, 0);

    font.setPixelSize(VICTORY_SIZE);
    QFontMetrics fm(font);
    // umístění nápisu VICTORY
    victory = new QGraphicsTextItem();
    victory->setFont(font);
    victory->setPos(( 6 * COLUMN_PADDING + CARD_W + 2*COLUMN_OFFSET_X - fm.width(QString("VICTORY!")))/2, COLUMN_OFFSET_Y + (CARD_H - fm.height())/2);

    this->addItem(victory);
    this->addItem(score);
    this->drawScore();
}

/*
 * Destruktor třídy SolitaireGui.
 */
SolitaireGui::~SolitaireGui(){
    this->clear();
    while(!backCards.empty()) {
        delete backCards[0];
        backCards.erase(backCards.begin());
    }
}

/*
 * Nová hra.
 */
void SolitaireGui::newGame(void) {
    this->clear();
    this->game->staffleCards();
    this->draw();
}

/*
 * Vyčistí plátno až na podklad.
 */
void SolitaireGui::clear(void) {
    while(!cards.empty()) {
        this->removeItem(cards[0]);
        delete cards[0];
        cards.erase(cards.begin());
    }
    for(int i = 0; i < 2; i++) {
        if(hints[i] != NULL) {
            this->removeItem(hints[i]);
            delete hints[i];
        }
        hints[i] = NULL;
    }
}

/*
 * Zobrazí nápovědu pomocí 2 žlutých rámečků.
 */
void SolitaireGui::hint(void) {
    this->draw();
    Move *m = game->getHint();

    if(m != NULL) {
        int x = m->getFrom();
        int y = m->getIdx();
        int n = x == 0 ? 0 : (*game)[x].size() - y - 1;

        hints[0] = this->addHint(x, y ,n);

        x = m->getTo();
        if(x != 0) {
            y = x > 7 || x == 0 ? 0 : (*game)[x].size()-1;
            y = y < 0 ? 0 : y;
            n = 0;

            hints[1] = this->addHint(x, y ,n);
        }
    }
}

/*
 * Vykreslení nápovědy.
 */
QGraphicsRectItem * SolitaireGui::addHint(int x, int y, int n) {
    QGraphicsRectItem *item;

    if(x == 0) {
        if(y == -1 ) {
            item = this->addRect(DECK_OFFSET_X - BORDER, DECK_OFFSET_Y - BORDER, CARD_W + BORDER+1, CARD_H + BORDER+1);
        } else {
            item = this->addRect(DECK_OFFSET_X + COLUMN_PADDING - BORDER, DECK_OFFSET_Y - BORDER, CARD_W + BORDER+1, CARD_H + BORDER+1);
        }
    } else if(1 <= x && x < 8) {
        item = this->addRect(COLUMN_OFFSET_X + (x-1) * COLUMN_PADDING - BORDER, COLUMN_OFFSET_Y + (y * ROW_PADDING) - BORDER, CARD_W + BORDER+1, (n * ROW_PADDING) + CARD_H + BORDER+1);
    } else {
        item = this->addRect(STORE_OFFSET_X + (x-8) * COLUMN_PADDING - BORDER, STORE_OFFSET_Y - BORDER, CARD_W + BORDER+1, CARD_H + BORDER+1);
    }

    item->setBrush(Qt::yellow);
    item->setPen(QPen(Qt::yellow));
    if(x == 0 && y != -1)
    {
        item->setZValue(10-y);
    }
    else
    {
        item->setZValue(y-1);
    }

    return item;
}

/*
 * Tah zpět.
 */
void SolitaireGui::undo(void) {
    game->undo();
    this->draw();
}

/*
 * Vykresleni karet na platno.
 */
void SolitaireGui::draw(void) {

    this->clear();
    this->drawScore();
    this->drawVictory();

    if(game->isDeckFlip()) {
        flipDeck->setZValue(1);
    } else {
        flipDeck->setZValue(-1);
    }

    std::vector<Card *> s;
    for(int i= 0; i < 12; ++i) {
        s = (*game)[i];
        for(unsigned int idx = 0; idx < s.size(); idx++) {
            CardGui *l =  new CardGui(s[idx], this);
            l->draw();
            cards.append(l);
        }
    }
}

/*
 * Vykreslení skóre.
 */
void SolitaireGui::drawScore(void)
{
    if(score != NULL) {
        score->setPlainText(QString("Score: ") + QString::number(game->getScore()));
    }
}

/*
 * Vykreslení konce hry.
 */
void SolitaireGui::drawVictory(void)
{
    if(victory != NULL) {
        if(this->game->isEnd()) {
            victory->setPlainText(QString("VICTORY!"));
        } else if(victory != NULL) {
            victory->setPlainText(QString(""));
        }
    }
}

/*
 * Smazání všech karet.
 */
void SolitaireGui::quitGame(void) {
    game->clear();
    this->draw();
}

/*
 * Dialogové okno pro uložení hry.
 */
void SolitaireGui::save(void) {
    QString fileName = QFileDialog::getSaveFileName();
    std::ofstream out(fileName.toUtf8().constData());
    if(out.good()) {
        out << *game;
        out.close();
    }
}

/*
 * Dialogové okno pro načtení hry.
 */
void SolitaireGui::load(void) {

    QFileDialog *dialog = new QFileDialog();
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    std::ifstream in(dialog->getOpenFileName().toUtf8().constData());
    if(in.good()) {
        this->clear();
        in >> *game;
        in.close();
    }
    this->draw();
}

void SolitaireGui::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    for(int i = 0; i < 2; i++) {
        if(hints[i] != NULL) {
            this->removeItem(hints[i]);
            delete hints[i];
        }
        hints[i] = NULL;
    }

    QGraphicsItem *i = itemAt(event->scenePos(), QTransform());

    if(i != 0 && game->isEnd() == false) {
        CardGui *card = dynamic_cast<CardGui*>(i);
        if(card != NULL) {
            if(card->getCol() == 0 && !card->isMoveable()) {
                game->rollDeck();
                this->draw();
            } else if(card->isMoveable()) {
                active = card;
                offset = event->scenePos() - active->pos();
            }
        }
    }
}
void SolitaireGui::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if(active != NULL) {
        int col = active->getCol();
        int row = active->getRow();

        int x = event->scenePos().x() - offset.x();
        int y = event->scenePos().y() - offset.y();
        for(auto &c : cards) {
            if(c->getCol() == col && c->getRow() >= row && !(col == 0 && c->getRow() != row)) {
                c->setPos(x, y + (c->getRow() - row)*30);
                c->setZValue(20 + (c->getRow() - row));
            }
        }
    }
}

void SolitaireGui::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if(this->active != NULL) {
        active->setZValue(this->items().last()->zValue() - 0.1);
        active->setPos(0, 0);
        QGraphicsItem *i = itemAt(event->scenePos(), QTransform());
        if(i != 0) {
            CardGui *card = static_cast<CardGui*>(i);
            game->move(active->getCol(), card->getCol(), active->getRow());
        }

        active = NULL;
        this->draw();
    }
}
void SolitaireGui::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event )
{
    if ( event->button() == Qt::LeftButton )
    {
        QGraphicsItem *i = itemAt(event->scenePos(), QTransform());
        if(i != 0) {
            CardGui *card = static_cast<CardGui*>(i);
            game->move(card->getCol());
            for(int i = 0; i < 2; i++) {
                if(hints[i] != NULL) {
                    this->removeItem(hints[i]);
                    delete hints[i];
                }
                hints[i] = NULL;
            }

            this->draw();
        }
    }
}

void SolitaireGui::keyPressEvent(QKeyEvent* event) {

    switch(event->key()) {
    case Qt::Key_H:
        this->hint();
        break;
    case Qt::Key_U:
        this->undo();
        break;
    case Qt::Key_N:
        this->newGame();
        break;
    case Qt::Key_Q:
        this->quitGame();
        break;
    case Qt::Key_S:
        this->save();
        break;
    case Qt::Key_L:
        this->load();
        break;
    case Qt::Key_Space:
        game->rollDeck();
        this->draw();
        break;
    default:
        break;
    }
}
