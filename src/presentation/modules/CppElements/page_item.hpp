#include <QPoint>
#include <QQuickItem>
#include <QString>
#include <memory>
#include "document_item.hpp"
#include "page.hpp"
#include "presentation_export.hpp"

namespace cpp_elements
{

class PRESENTATION_LIBRARY PageItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(
        DocumentItem* document READ getDocument WRITE setDocument CONSTANT)
    Q_PROPERTY(
        int implicitWidth READ getImplicitWidth NOTIFY implicitWidthChanged)
    Q_PROPERTY(
        int implicitHeight READ getImplicitHeight NOTIFY implicitHeightChanged)
    Q_PROPERTY(int pageNumber READ getPageNumber WRITE setPageNumber CONSTANT)
    Q_PROPERTY(bool colorInverted WRITE setColorInverted)


public:
    PageItem();

    int getImplicitWidth() const;
    int getImplicitHeight() const;

    DocumentItem* getDocument() const;
    void setDocument(DocumentItem* newDocument);

    int getPageNumber() const;
    void setPageNumber(int newCurrentPage);

    void setColorInverted(bool newColorInverted);
    Q_INVOKABLE void setSelection(int beginX, int beginY, int endX, int endY);
    Q_INVOKABLE void removeSelection();
    Q_INVOKABLE void copySelectedText();
    Q_INVOKABLE bool pointIsAboveText(int x, int y);

private slots:
    void updateZoom(float newZoom);

protected:
    void geometryChange(const QRectF& newGeometry,
                        const QRectF& oldGeometry) override;
    QSGNode* updatePaintNode(QSGNode* node, UpdatePaintNodeData* _) override;

private:
    void paintSelectionOnPage(QPainter& painter);
    void generateSelection();

    DocumentItem* m_document = nullptr;
    std::unique_ptr<application::core::Page> m_page;
    int m_currentPage = 0;
    bool m_firstTimeColorInverted = true;
    QPointF m_selectionStart;
    QPointF m_selectionEnd;
};

}  // namespace cpp_elements
