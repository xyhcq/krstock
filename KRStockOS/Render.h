#ifndef Render_h__
#define Render_h__

#include "klinewidget.h"
#include "HQWidget.h"
#include "AmountAnalyzeWidget.h"
#include "FenBiWidget.h"
#include "StockToolsWiget.h"
#include "Widget/SplitterColWidget.h"
#include "Widget/SplitterRowWidget.h"


enum{ KLineAnalyze_ID_KLineWidget = 911,KLineAnalyze_ID_HQWidget,KLineAnalyze_ID_AmountAnalyzeWidget,KLineAnalyze_ID_FenBiWidget,KLineAnalyze_ID_ToolsWidget};


class CRender
{
public:
	virtual void Create(CWnd * pWnd) =0;
	virtual void SetLabel(const char *  szLabel)=0;
	virtual void AdjuestWidget(int x, int y, int cx, int cy)=0;
	
};



class CKLineAnalyze : public CRender
{
public:
	CKLineAnalyze();
	void Create(CWnd * pWnd);
	void SetLabel(const char * szLabel);
	void AdjuestWidget(int x, int y, int cx, int cy);
	BOOL PreTranslateMessage(CWnd * pWnd,MSG* pMsg);
private:
	CKLineWidget			m_KLineWidget;
	CHQWidget				m_HQWidget;
	CAmountAnalyzeWidget	m_AmountAnalyzeWidget;
	CFenBiWidget			m_FenBiWidget;
	CStockToolsWiget		m_StockToolsWiget;
	CRect					m_Rect;

	CSplitterColWidget	    m_SplitterColWidget;
	CSplitterRowWidget      m_SplitterRowidget;
	CSplitterRowWidget      m_SplitterSubRowidget;

	BOOL					m_bFirst;
};



#endif // Render_h__