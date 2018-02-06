#include "logic.h"
#include <iostream>

struct Logic::Impl
{
  QList<Figure> figures;
  QList<QList <Figure> > history;

  int findByPosition(int x, int y);
  int findPlayer(int x, int y);
  void addToHistory();
};

void Logic::Impl::addToHistory(){
    history.append(figures);
}

bool Logic::getCurrentTurn() {
    return current_turn;
}

int Logic::saveFile(){
    QFile file("saved_file");
    if (!file.open(QIODevice::WriteOnly)) {
        std::cerr << "Cannot open file for writing: "
                  << qPrintable(file.errorString()) << std::endl;
        return -1;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_9);
    out << impl->history;
    file.close();
    return 0;
}

void Logic::loadFromHistory(){
    beginResetModel();
    int j = impl->history.count();
    QList<QList <Figure> >::iterator i;
    for(i = impl->history.end(); j-- != moves; i--)
        continue;
    impl->figures = *i;
    while(++j < impl->history.count() - 1)
        impl->history.removeAt(j);
    endResetModel();
}

void Logic::iterateOverHistory(){
    int j = impl->history.count();
    if (moves >= 1 && moves < j)
    {
        beginResetModel();
        QList<QList <Figure> >::iterator i;
        for(i = impl->history.end(); j-- != moves; i--)
            continue;
        impl->figures = *i;
        endResetModel();
    }
    else if (moves < 1)
        moves = 1;
    else
        moves = j - 1;
}

void Logic::prevMove(){
    moves--;
    iterateOverHistory();
}

void Logic::nextMove(){
    moves++;
    iterateOverHistory();
}

void Logic::loadGame(){
    beginResetModel();
    clear();
    impl->figures = impl->history.last();
    moves = impl->history.count() - 1;
    endResetModel();
}

int Logic::loadFile(){
    QFile file("saved_file");
    if (!file.open(QIODevice::ReadOnly)) {
            std::cerr << "Cannot open file for reading: "
                      << qPrintable(file.errorString()) << std::endl;
            return -1;
        }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_9);
    in >> impl->history;
    file.close();
    loadGame();
    return 0;
}

QDataStream &operator>>(QDataStream& in, QList<QList <Figure> >& history){
    QList<Figure> tmpLst;
    Figure tmpFigure;

    QList<QList <Figure> >::iterator i;
    QList<Figure>::iterator j;
    for (i = history.begin(); i != history.end(); ++i)
    {
        tmpLst = *i;
        for (j = tmpLst.begin(); j != tmpLst.end(); ++j)
        {
            tmpFigure = *j;
            in >> tmpFigure.type;
            in >> tmpFigure.x;
            in >> tmpFigure.y;
            in >> tmpFigure.p;
        }
    }
    return in;
}

QDataStream &operator<<(QDataStream& out, QList<QList <Figure> >& history){
    QList<Figure> tmpLst;
    Figure tmpFigure;

    QList<QList <Figure> >::iterator i;
    QList<Figure>::iterator j;
    for (i = history.begin(); i != history.end(); ++i)
    {
        tmpLst = *i;
        for (j = tmpLst.begin(); j != tmpLst.end(); ++j)
        {
            tmpFigure = *j;
            out << tmpFigure.type;
            out << tmpFigure.x;
            out << tmpFigure.y;
            out << tmpFigure.p;
        }
    }
    return out;
}

int Logic::Impl::findPlayer(int x, int y){
    for (int i(0); i<figures.size(); ++i){
        if (figures[i].x == x && figures[i].y == y)
            return figures[i].p;
    }
    return -1;
}

int Logic::Impl::findByPosition(int x, int y) {
  for (int i(0); i<figures.size(); ++i) {
    if (figures[i].x != x || figures[i].y != y ) { 
      continue; 
    }
    return i;    
  }
  return -1;
}

void Logic::newGame(){
    beginResetModel();
    impl->figures << Figure { 0, 0, 1, 1 };
    impl->figures << Figure { 0, 1, 1, 1 };
    impl->figures << Figure { 0, 2, 1, 1 };
    impl->figures << Figure { 0, 3, 1, 1 };
    impl->figures << Figure { 0, 4, 1, 1 };
    impl->figures << Figure { 0, 5, 1, 1 };
    impl->figures << Figure { 0, 6, 1, 1 };
    impl->figures << Figure { 0, 7, 1, 1 };
    impl->figures << Figure { 1, 0, 6, 2 };
    impl->figures << Figure { 1, 1, 6, 2 };
    impl->figures << Figure { 1, 2, 6, 2 };
    impl->figures << Figure { 1, 3, 6, 2 };
    impl->figures << Figure { 1, 4, 6, 2 };
    impl->figures << Figure { 1, 5, 6, 2 };
    impl->figures << Figure { 1, 6, 6, 2 };
    impl->figures << Figure { 1, 7, 6, 2 };
    impl->figures << Figure { 2, 0, 0, 1 };
    impl->figures << Figure { 2, 7, 0, 1 };
    impl->figures << Figure { 3, 0, 7, 2 };
    impl->figures << Figure { 3, 7, 7, 2 };
    impl->figures << Figure { 4, 1, 0, 1 };
    impl->figures << Figure { 4, 6, 0, 1 };
    impl->figures << Figure { 5, 1, 7, 2 };
    impl->figures << Figure { 5, 6, 7, 2 };
    impl->figures << Figure { 6, 5, 0, 1 };
    impl->figures << Figure { 6, 2, 0, 1 };
    impl->figures << Figure { 7, 2, 7, 2 };
    impl->figures << Figure { 7, 5, 7, 2 };
    impl->figures << Figure { 8, 3, 0, 1 };
    impl->figures << Figure { 9, 3, 7, 2 };
    impl->figures << Figure { 10, 4, 0, 1 };
    impl->figures << Figure { 11, 4, 7, 2 };
    impl->addToHistory();
    current_turn = 0;
    endResetModel();
}

Logic::Logic(QObject *parent)
  : QAbstractListModel(parent)
  , impl(new Impl())
{
    newGame();
}

Logic::~Logic() {
}

int Logic::boardSize() const {
  return BOARD_SIZE;
}

int Logic::rowCount(const QModelIndex & ) const {
  return impl->figures.size(); 
}

QHash<int, QByteArray> Logic::roleNames() const { 
  QHash<int, QByteArray> names;
  names.insert(Roles::Type      , "type");
  names.insert(Roles::PositionX , "positionX");
  names.insert(Roles::PositionY , "positionY");
  return names;
}

QVariant Logic::data(const QModelIndex & modelIndex, int role) const { 
  if (!modelIndex.isValid()) {
    return QVariant();
  }
    
  int index = static_cast<int>(modelIndex.row());
  
  if (index >= impl->figures.size() || index < 0) {
    return QVariant();
  }

  Figure & figure = impl->figures[index];
    
  switch (role) {
    case Roles::Type     : return figure.type;
    case Roles::PositionX: return figure.x;
    case Roles::PositionY: return figure.y;
  }
  return QVariant(); 
}

void Logic::clear() {
  beginResetModel();
  impl->figures.clear();
  endResetModel();
}

bool Logic::move(int fromX, int fromY, int toX, int toY, int type) {
  if (toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= BOARD_SIZE || (impl->findPlayer(toX, toY) == impl->findPlayer(fromX, fromY))) {
    return false;
  }
  int toIndex = impl->findByPosition(toX, toY);
  int fromIndex = impl->findByPosition(fromX, fromY);
  if ((type % 2 > 0 && current_turn == 1) || (type % 2 == 0 & current_turn == 0))
      return false;
  else if (type < 2)
  {
      if (pawnMove(type, fromX, fromY, toX, toY, toIndex, fromIndex) == false)
          return false;
  }
  else if (type > 1 && type < 4)
  {
      if (rookMove(fromX, fromY, toX, toY, toIndex, fromIndex) == false)
          return false;
  }
  else if (type > 3 && type < 6)
  {
      if (knightMove(fromX, fromY, toX, toY, toIndex, fromIndex) == false)
          return false;
  }
  else if (type > 5 && type < 8)
  {
      if (bishopMove(fromX, fromY, toX, toY, toIndex, fromIndex) == false)
          return false;
  }
  else if (type > 7 && type < 10)
  {
      if (queenMove(fromX, fromY, toX, toY, toIndex, fromIndex) == false)
          return false;
  }
  else if (type > 9 && type < 12)
  {
      if (kingMove(fromX, fromY, toX, toY, toIndex, fromIndex) == false)
          return false;
  }
  impl->addToHistory();
  current_turn = (current_turn == 0) ? 1 : 0;
  return true;
}

void Logic::update(int x, int y, int index)
{
    beginResetModel();
    impl->figures[index].x = x;
    impl->figures[index].y = y;
    QModelIndex topLeft = createIndex(index, 0);
    QModelIndex bottomRight = createIndex(index, 0);
    emit dataChanged(topLeft, bottomRight);
    endResetModel();
}

bool Logic::checkOverlap(int fromX, int fromY, int toX, int toY)
{
    if (fromY <= toY){
        while(fromY++ < toY - 1){
            if (fromX <= toX){
                if (fromX < toX - 1)
                    fromX++;
                if (impl->findByPosition(fromX, fromY) >= 0)
                    return true;
            }
            else if (fromX > toX){
                if (fromX > toX + 1)
                    fromX--;
                 if (impl->findByPosition(fromX, fromY) >= 0)
                    return true;
            }
        }
    }
    else if (fromY > toY){
        while(fromY-- > toY + 1){
            if (fromX <= toX){
                if (fromX < toX - 1)
                    fromX++;
                if (impl->findByPosition(fromX, fromY) >= 0)
                    return true;
            }
            else if (fromX > toX){
                if (fromX > toX + 1)
                    fromX--;
                 if (impl->findByPosition(fromX, fromY) >= 0)
                    return true;
                }
            }
        }
    return false;
}

bool Logic::pawnMove(int type, int fromX, int fromY, int toX, int toY, int toIndex, int fromIndex){
    bool blackPawnMove = (type == 0 && fromX == toX && ((toY - fromY == 1) || (fromY == 1 && toY == 3)) && toIndex == -1);
    bool whitePawnMove = (type == 1 && fromX == toX && ((fromY - toY == 1) || (fromY == 6 && toY == 4)) && toIndex == -1);
    bool blackPawnHit = (type == 0 && ((toX - fromX) == 1 || (fromX - toX == 1)) && toIndex > 0 && toY - fromY == 1);
    bool whitePawnHit = (type == 1 && ((toX - fromX) == 1 || (fromX - toX == 1)) && toIndex > 0 && fromY - toY == 1);
    if (whitePawnMove || blackPawnMove)
    {
        update(toX, toY, fromIndex);
        return true;
    }
    else if (whitePawnHit || blackPawnHit)
    {
       update(-1, -1, toIndex);
       update(toX, toY, fromIndex);
       return true;
    }
    return false;
}

bool Logic::rookMove(int fromX, int fromY, int toX, int toY, int toIndex, int fromIndex){
    if ((toX == fromX || toY == fromY) && toIndex == -1 && (checkOverlap(fromX, fromY, toX, toY) == false))
    {
        update(toX, toY, fromIndex);
        return true;
    }
    else if ((toX == fromX || toY == fromY) && toIndex > 0 && checkOverlap(fromX, fromY, toX, toY) == false)
    {
        update(-1, -1, toIndex);
        update(toX, toY, fromIndex);
        return true;
    }
    return false;
}

bool Logic::knightMove(int fromX, int fromY, int toX, int toY, int toIndex, int fromIndex){
    if ((((toY - fromY == 2 || fromY - toY == 2) && ((toX - fromX == 1) || (fromX - toX == 1))) ||
         ((toY - fromY == 1 || fromY - toY == 1) && ((toX - fromX == 2) || (fromX - toX == 2)))) && toIndex == -1){
        update(toX, toY, fromIndex);
        return true;
    }
    else if ((((toY - fromY == 2 || fromY - toY == 2) && ((toX - fromX == 1) || (fromX - toX == 1))) ||
              ((toY - fromY == 1 || fromY - toY == 1) && ((toX - fromX == 2) || (fromX - toX == 2)))) && toIndex > 0){
        update(-1, -1, toIndex);
        update(toX, toY, fromIndex);
        return true;
    }
    return false;
}

bool Logic::bishopMove(int fromX, int fromY, int toX, int toY, int toIndex, int fromIndex){
    int diagonal = (fromY > toY) ? fromY - toY : toY - fromY;
    if ((fromX - toX == diagonal || toX - fromX == diagonal) && toIndex == -1 && checkOverlap(fromX, fromY, toX, toY) == false){
        update(toX, toY, fromIndex);
        return true;
    }
    else if ((fromX - toX == diagonal || toX - fromX == diagonal) && toIndex > 0 && checkOverlap(fromX, fromY, toX, toY) == false){
        update(-1, -1, toIndex);
        update(toX, toY, fromIndex);
        return true;
    }
    return false;
}

bool Logic::queenMove(int fromX, int fromY, int toX, int toY, int toIndex, int fromIndex){
    if (toIndex == -1 && checkOverlap(fromX, fromY, toX, toY) == false)
    {
        update(toX, toY, fromIndex);
        return true;
    }
    else if (toIndex > 0 && checkOverlap(fromX, fromY, toX, toY) == false){
        update(-1, -1, toIndex);
        update(toX, toY, fromIndex);
        return true;
    }
    return false;
}

bool Logic::kingMove(int fromX, int fromY, int toX, int toY, int toIndex, int fromIndex){
    if ((fromX - toX == 1 || toX - fromX == 1 || fromX == toX) && (fromY == toY || fromY - toY == 1 || toY - fromY == 1) && toIndex == -1){
        update(toX, toY, fromIndex);
        return true;
    }
    else if ((fromX - toX == 1 || toX - fromX == 1 || fromX == toX) && (fromY == toY || fromY - toY == 1 || toY - fromY == 1) && toIndex > 0){
        update(-1, -1, toIndex);
        update(toX, toY, fromIndex);
        return true;
    }
    return false;
}

