#pragma once
#include <memory>
#include <QAbstractListModel>
#include <QList>
#include <QByteArray>
#include <QHash>
#include <iostream>

struct Figure
{
  int type;
  int x;
  int y;
  int p;
};

class Logic: public QAbstractListModel
{
    Q_OBJECT
public:

    enum GlobalConstants {
        BOARD_SIZE = 8
    };

    enum Roles {
        Type = Qt::UserRole,
        PositionX,
        PositionY,
    };
    
public:
    explicit Logic(QObject *parent = 0);
    ~Logic();

    Q_PROPERTY(int boardSize READ boardSize CONSTANT);
    int boardSize() const;

    Q_INVOKABLE void clear();
    Q_INVOKABLE bool move(int fromX, int fromY, int toX, int toY, int type);
    Q_INVOKABLE void newGame();
    Q_INVOKABLE int saveFile();
    Q_INVOKABLE int loadFile();
    Q_INVOKABLE void prevMove();
    Q_INVOKABLE void nextMove();
    Q_INVOKABLE void loadFromHistory();
    Q_INVOKABLE bool getCurrentTurn();

protected:
    int rowCount(const QModelIndex & parent) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool pawnMove(int type, int fromX, int fromY, int toX, int toY, int toIndex, int fromIndex);
    bool rookMove(int fromX, int fromY, int toX, int toY, int toIndex, int fromIndex);
    bool knightMove(int fromX, int fromY, int toX, int toY, int toIndex, int fromIndex);
    bool bishopMove(int fromX, int fromY, int toX, int toY, int toIndex, int fromIndex);
    bool queenMove(int fromX, int fromY, int toX, int toY, int toIndex, int fromIndex);
    bool kingMove(int fromX, int fromY, int toX, int toY, int toIndex, int fromIndex);
    void update(int x, int y, int index);
    bool checkOverlap(int fromX, int fromY, int toX, int toY);
    void loadGame();
    void iterateOverHistory();

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
    int moves;
    bool current_turn;
};

QDataStream &operator<<(QDataStream& out, QList<QList <Figure> >& history);
QDataStream &operator>>(QDataStream& in, QList<QList <Figure> >& history);
