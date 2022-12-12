#include <QCoreApplication>
#include <QImage>
#include <QPainter>
#include <QRandomGenerator>

#include <QDebug>

const int w = 10000;
const int h = 10000;

double r(QPointF a, QPointF o) {
  return sqrt((a.x() - o.x())*(a.x()- o.x())
              + (a.y() - o.y())*(a.y() - o.y()));
}

double phi (QPointF a,QPointF o) {
  return (a.y() - o.y())/r(a, o);
}

long double angle(QPointF a, QPointF b, QPointF c) {
  return  ((a.x() - b.x())*(c.y() - b.y()) - (c.x() - b.x())*(a.y() - b.y()))/
      (r(a, b) + r(c, b));
}
//Рандомное заполнение точек
QVector<QPointF> filling(QVector<QPointF> pnts, int n) {
  const auto random = QRandomGenerator::global();

  for (int i = 0; i < n; ++i) {
    pnts.push_back(QPointF(random->bounded(w), random->bounded(h)));
  }

  return pnts;
}

QVector<QPointF> Graham(QVector<QPointF> points, int nPoints) {

   //Поиск крайней левой точки
  for (int i = 1; i < nPoints; ++i)  {
    if (points[i].x() < points[0].x())
      std::swap(points[i], points[0]);
  }

  //Сортировка всех точек по левизне относительно нулевой
  std::sort(points.begin()+1, points.end(), [points](QPointF a,QPointF b) {
    if (phi(a, points[0]) != phi(b, points[0]))
      return phi(a, points[0]) < phi(b, points[0]);
    else return r(a, points[0]) < r(b, points[0]);});

  QVector<QPointF> MBO{points};

  //Создание массива точек МВО
  int i = 2;
  while (i < MBO.size() - 1) {
    while (i < MBO.size() - 1 && angle(MBO[i - 1], MBO[i], MBO[i + 1]) > 0) {
        MBO.remove(i);
        i--;
    }
    i++;
  }
  MBO.push_back(points[nPoints-1]);

  return MBO;

}

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  QImage img(w, h, QImage::Format_RGB888);
  img.fill(QColor(0, 0, 0));

  QVector<QPointF> points;

  const int nPoints = 16050;

  points = filling(points, nPoints);

  QPainter painter(&img);

  QPen pen;

  pen.setColor(QColor(255, 255, 255));
  pen.setWidth(7);
  pen.setCapStyle(Qt::RoundCap);
  painter.setPen(pen);

  painter.drawPoints(points);

  img.save("points.jpg");

  pen.setColor(QColor(0, 0, 255));
  pen.setWidth(3);
  painter.setPen(pen);

  painter.drawPolygon(Graham(points, nPoints));

  img.save("pointsEND.jpg");

  return 0;
}















































