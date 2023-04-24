/*!
 * \file    cardgui.h
 * \authors Karel ONDŘEJ, FIT
 * \authors David SMÉKAL, FIT
 * \date    5. 5. 2017
 * \brief   Grafické reprezentace karty.
 */

#ifndef CARDGUI_H
#define CARDGUI_H

#include <QGraphicsPixmapItem>
#include <solitairegui.h>

#define CARD_H          120     //!< Výška karty
#define CARD_W          83      //!< Šířka karty
#define BORDER          2       //!< Tloušťka okraje
#define ROW_PADDING     30      //!< Svislý rozestup mezi kartami
#define COLUMN_PADDING  100     //!< Horizontální rozestup mezi kartami

#define DECK_OFFSET_X   20      //!< Horizontální odsazení balíčku
#define DECK_OFFSET_Y   25      //!< Svislé odsazení balíčku
#define COLUMN_OFFSET_X (DECK_OFFSET_X)     //!< Horizontální odsazení hrací části
#define COLUMN_OFFSET_Y ((DECK_OFFSET_Y) + (CARD_H) + (ROW_PADDING))    //!< Svislé odsazení hrací části
#define STORE_OFFSET_X  ((DECK_OFFSET_X) + 3*(COLUMN_PADDING))      //!< Horizontální odsazení finálnívh balíčků
#define STORE_OFFSET_Y  (DECK_OFFSET_Y)     //!< Svislé odsazení finálních balíčků

/*!
 * \brief Konstanty pro karty.
 */
namespace Solitaire {
    /*! \brief Ikony karet */
    const QString CARDS[][4] = {
        {":/ace_of_clubs.png"  , ":/ace_of_spades.png"  , ":/ace_of_diamonds.png"  , ":/ace_of_hearts.png"  },
        {":/2_of_clubs.png"    , ":/2_of_spades.png"    , ":/2_of_diamonds.png"    , ":/2_of_hearts.png"    },
        {":/3_of_clubs.png"    , ":/3_of_spades.png"    , ":/3_of_diamonds.png"    , ":/3_of_hearts.png"    },
        {":/4_of_clubs.png"    , ":/4_of_spades.png"    , ":/4_of_diamonds.png"    , ":/4_of_hearts.png"    },
        {":/5_of_clubs.png"    , ":/5_of_spades.png"    , ":/5_of_diamonds.png"    , ":/5_of_hearts.png"    },
        {":/6_of_clubs.png"    , ":/6_of_spades.png"    , ":/6_of_diamonds.png"    , ":/6_of_hearts.png"    },
        {":/7_of_clubs.png"    , ":/7_of_spades.png"    , ":/7_of_diamonds.png"    , ":/7_of_hearts.png"    },
        {":/8_of_clubs.png"    , ":/8_of_spades.png"    , ":/8_of_diamonds.png"    , ":/8_of_hearts.png"    },
        {":/9_of_clubs.png"    , ":/9_of_spades.png"    , ":/9_of_diamonds.png"    , ":/9_of_hearts.png"    },
        {":/10_of_clubs.png"   , ":/10_of_spades.png"   , ":/10_of_diamonds.png"   , ":/10_of_hearts.png"   },
        {":/jack_of_clubs.png" , ":/jack_of_spades.png" , ":/jack_of_diamonds.png" , ":/jack_of_hearts.png" },
        {":/queen_of_clubs.png", ":/queen_of_spades.png", ":/queen_of_diamonds.png", ":/queen_of_hearts.png"},
        {":/king_of_clubs.png" , ":/king_of_spades.png" , ":/king_of_diamonds.png" , ":/king_of_hearts.png" }
    };
    const QString BACK_SIDE{":/back_side.png"};     //!< Zadní strana karty
    const QString LAST_CARD{":/last_card.png"};     //!< Karta otočení balíčku
    const QString EMPTY_CARD{":/empty_card.png"};   //!< Podkladová karta
}

class SolitaireGui;


/*!
 * \brief Třída graficky reprezentující kartu.
 */
class CardGui : public QGraphicsPixmapItem, protected Card
{
private:
    SolitaireGui *parent;   /*! Plátno, na kterém je karta vykreslená. */

public:
    /*!
     * \brief Konstruktor třídy Card.
     *
     * Třída reprezentuje kartu card.
     * \param card   Karta
     * \param parent Rodič třídy
     */
    CardGui(Card *card, SolitaireGui *parent);
    /*!
     * \brief Konstruktor třídy Card.
     *
     * Argument icon udává vzhled karty. Karta je umístěna na pozici balíčku a je nepohlibivá. Slouží k vykreslení speciálních karet, jako kartu na umístění hromádky.
     * \param icon    Ikona karty
     * \param parent  Plátno, na kterém je karta vykreslena
    */
    CardGui(QIcon icon, SolitaireGui *parent);
    /*!
     * \brief Vykreslení karty.
     */
    void draw(void);

    using Card::setPosition;

    using Card::setMoveable;
    using Card::isMoveable;

    using Card::getCol;
    using Card::getRow;
};

#endif // CARDGUI_H
