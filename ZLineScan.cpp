#include "stdafx.h"
#include "vtkPolyData.h"
#include "vtkDoubleArray.h"

const char cGInstruct[][4] = { "G00", "G01" };

CZGrid::CZGrid(void)
{
	lines[scanX].setType(scanX);
	lines[scanY].setType(scanY);
}

void CZGrid::make(
	ILineScan* algor,
	const CZLineScanSetting &p_setting,
	vtkSmartPointer<vtkPolyData>p_slice)
{
	algor->exec(p_setting, p_slice, lines[scanX]);
	algor->exec(p_setting, p_slice, lines[scanY]);
}

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

namespace ZLineScanAlgorithm
{
	//
	//implement of algorithm ILineScan : CZLineScan
	//
	class CZLineScan : public ILineScan
		//see <<computer graphy>> P181
	{
	public:
		void exec(
			const CZLineScanSetting &p_setting, 
			vtkSmartPointer<vtkPolyData> p_slice,
			CZScanLines &p_lines);
	};

	CZLineScan algorLineScan;

	//
	//implement of CZLineScan
	//
	const float cLOMMinScanLen=0.5f;

	struct CScanEdge
	{
		float x, dx;//x���ꡢx����
		int ymaxID;//�����ߵ�Y���ֵ
	};

	bool inline operator>(const CScanEdge &e1, const CScanEdge &e2)
	{
		return e1.x>e2.x;
	};

	typedef CZSortArray<CScanEdge> CScanEdges;
	typedef CZArray<CScanEdges, true> CActiveEdgeList;

	class CSmartSubCords : public CZSubCords
	{
	public:
		CSmartSubCords(const CZLineScanSetting *p_pSetting) 
			: CZSubCords(cEstScanLineEdgesCount, -cEstScanLineEdgesCount*2),
			  m_pSetting(p_pSetting), m_bFirstNMLremoved(false) {;};
		void newScan(float p_gate) { m_maxGate=p_gate; setSize(0); m_bFirstNMLremoved=false;};
		bool bInLine(void) const { return size()%2!=0; };//ɨ���������ż������������һ��ɨ���߶���
		void finish(CZScanLines &p_lines, CZScanLineType p_scanType, float p_mainCord);
			//process subCords and add to p_linse
		void add(const float &p_newCord);

	protected:	//inner usage
		void checkLastPair(void);

	protected:	//hide invalid base methods
		float& add(void);

		//data member
		const CZLineScanSetting *m_pSetting;
		float m_maxGate;
		bool m_bFirstNMLremoved;	//NML is non-main line
	};

	const float cMinGate=1e-1f;

	void CSmartSubCords::checkLastPair(void)//������һ��ɨ���߽ڵ��Ƿ�̫С��̫��
	{
		float len=(float)fabs(at(size()-2)-at(size()-1));
		if ( ! ( (len>cMinGate) && (len<m_maxGate) ) )
			//delete invalid formal line then
		{
			removeTop(2);
		}
	}

	void CSmartSubCords::add(const float &p_newCord)
	{
		if ( (!m_bFirstNMLremoved) && (m_maxGate<1e5) && (size()==2) /*&& ((m_data[1]-m_data[0])<30)*/ )
			//if is a non-main scan line and it's the first line and it's shorter than 30mm
		{
			removeTop(2);
			m_bFirstNMLremoved=true;
		}

		if ((!bInLine()) && (!bEmpty()))	//there is already an edge pair, try to add new point
		{
			if (bEqual(top(), p_newCord, cMinGate)) //new line is attched with the forml line, actually one line
			{
				removeTop();
			}
			else //a truely new line start
			{
				//check the formal line first;
				checkLastPair();

				//now add the new line start 
				CZSubCords::add(p_newCord);
			}
		}
		else	//add end point of a line pair
			CZSubCords::add(p_newCord);

	};

	void CSmartSubCords::finish(CZScanLines &p_lines, CZScanLineType p_scanType, float p_mainCord)
	{
		if (bInLine()) removeTop();
		else if (!bEmpty()) 
		{
			if ( (m_maxGate<1e5) /*&& ((m_data[size()-1]-m_data[size()-2])<30)*/ )//˵��ʹ����multiScan
				//if in non-main scan line, last line shorter than 30mm
				removeTop(2);
			else checkLastPair();
		}

		if (bEmpty()) return;

		CZScanLine &newLine=p_lines.add();
		newLine.init(p_scanType, p_mainCord);
		CZSubCords &cords=newLine.subCords;
		cords.setMemSize(size());
		if ( (m_pSetting->bBiDirection) && (p_lines.topI()%2==1) )
			//ɨ������Ҫ���ʹ�������������
			//								��							
			//								\/
			//				����������������
			//reverse copy
		{
			for(int i=size()-1; i>=0; i--)
				cords.add(at(i));
		}
		else //order copy
		{
			for(int i=0; i<size(); i++)
				cords.add(at(i));
		}
	}

	void CZLineScan::exec(
			const CZLineScanSetting &p_setting, 
			 vtkSmartPointer<vtkPolyData> p_slice,
			CZScanLines &p_lines)
	{

		int num = p_slice->GetNumberOfCells();
		int num2 = p_slice->GetNumberOfPoints();

		//1. calc parameter
		p_lines.setSize(0);
		assert( (p_lines.type()==scanX) || (p_lines.type()==scanY) );
		CZScanLineType type=p_lines.type();
		assert(p_setting.scanType!=scanUnknown);
		if ((p_setting.scanType!=scanBoth) && (p_setting.scanType!=type)) return;

		assert(p_setting.multiScanPower>=0);
		assert(p_setting.multiScanPower<10);
		int subDiv=1 << p_setting.multiScanPower;//�൱������2^(p_setting.multiScanPower)��ֵ
		float intver=float(p_setting.intver/subDiv);

		//xstart��һ����x���꣡����ʼ�ձ�ʾɨ�跽��y��ʾɨ���ߵ�������
		float xStart=val(p_setting.rect.min, type);
		float xEnd=val(p_setting.rect.max, type);
		float yStart=val(p_setting.rect.min, type+1);
		float yEnd=val(p_setting.rect.max, type+1);

		if (p_setting.RPMType==rtLOM)
		{
			yStart+=intver/2;
			yEnd-=1.0f;
		}

		int lineCount=truncLarge((yEnd-yStart)/intver);//��ȡ����ȡ��������
		p_lines.init(max(lineCount, 0));
		if (lineCount<=0) return;

		//2. setup active list
		CActiveEdgeList activeList(lineCount, atSized);//������������Yֵ��������Ա������һά���飻
		for (int i=0; i<activeList.size(); i++)
			activeList[i].init(0, -cEstScanLineEdgesCount*1);//���������е�Ԫ���ǻ��Ա߱�CScanEdges

		for (int i=0; i< p_slice->GetNumberOfCells(); i++)//���������ϵ����ring
		{
			vtkSmartPointer<vtkIdList> pointPair = vtkSmartPointer<vtkIdList>::New();
			p_slice->GetCellPoints(i, pointPair);
			int id1, id2;
			id1 = pointPair->GetId(0);
			id2 = pointPair->GetId(1);
	// 		double p1[3] = (p_slice->GetPoint(id1));
	// 		double p2[3] = (p_slice->GetPoint(id2));
			CPoint3f point1, point2;
		
			point1.x = (p_slice->GetPoint(id1))[0]; point1.y = (p_slice->GetPoint(id1))[1]; point1.z = (p_slice->GetPoint(id1))[2];
			point2.x = (p_slice->GetPoint(id2))[0]; point2.y = (p_slice->GetPoint(id2))[1]; point1.z = (p_slice->GetPoint(id2))[2];
			//calc y1, y2
			float y1 = val(point1, type + 1);
			float y2 = val(point2, type + 1);
			bool bSwap=(y1>y2);		//make sure p2 is higher than p1
			if (bSwap) swapData(y1, y2);//��֤Y2�������ߵ�Yֵ����

			//deal y1...
			//if y1 in scan line aera id(n, n+1], set its line id=n+1
			int lineIDStart=truncLarge((y1-yStart)/intver);
			if (lineIDStart<0) lineIDStart=0;
			else if (lineIDStart>=activeList.size()) continue;	//out of scan aera

			//deal y2...
			//if y2 in scan line aera id(n, n+1], set its line id=n
			int lineIDEnd=truncLarge((y2-yStart)/intver)-1;
			if (y2==(yStart+lineIDEnd*intver)) lineIDEnd--;	//down dither
			//Y ��ɨ���ߵ������������ϱ��¿�


			//tell ring line from case(lineIDEnd<lineIDStart)�����ϱ��¿������ᵼ������λ��ɨ�����ϵ��߶���Ϊ�ǿ��Ժ������
			float touchY=yStart+lineIDStart*intver;
			if ( (y1==touchY) && (y2==y1) && (p_setting.ringLineDealing!=clDefault) )
				//Note:
				//  y1=touchY,means (y1-yStart)/intver is a integer,y1������λ��һ��ɨ������
				//  if y1=y2=touchY, it's a ring line,
				//	it has the spec that lineIDEnd<lineIDStart, 
				//	but it should be carefully dealt on the scan process
			{
				//cout<<"aa"<<endl;
			}
			else
			{
			
				if (lineIDEnd<lineIDStart) continue;
				//������������������ǣ�y1��y2��λ��ͬһ��ɨ���ߣ�n��n+1�������ڣ�
			}

			//calc x1,x2
			float x1 = val(point1, type);
			float x2 = val(point2, type);
			if (bSwap) swapData(x1, x2);
			
			//add new edge
			CScanEdge edge;
			if (y1==y2)	//it must be ring line on touchY (y1=y2=touchY)���������ϵ��ų����������ֻҪ��������λ��ͬһYֵ����Ȼ����һ��ɨ������
			{
				edge.x=x1;
				edge.dx=x2;	//for ring line, store x2 on dx
			}
			else
			{
				float d=(x2-x1)/(y2-y1);
				edge.x=float( d*(touchY-y1) + x1 );//��ʼ��p1��x��x1�ƶ���touchY���ڵ�x
				edge.dx=float( d*intver );
			}
			edge.ymaxID=lineIDEnd;
			activeList[lineIDStart].add(edge);//�����뵱ǰɨ���߸߶�Y�Ļ��Ա���
		}

		//3.scan now
		CSmartSubCords cords(&p_setting);
		bool bScanRing=(p_setting.ringLineDealing==clGo);
			//Note: this boolean is used only for ring line, 
			//		if p_setting.ringLineDealing is clDefault, no ring line will be added 
			//			at setup active list process. so for ring line, there is no clDefault state
		for (int i=0; i<activeList.size(); i++)
		{
			//ÿ�ο�ʼһ�����ѭ����ʾ��ʼ����һ���߶�Ϊi��ɨ���ߵ�ɨ�裻
			//
			//calc gate for maxLineLen
			//����ʹ��multiScanʱ��subdiv=1��
			//ʹ��multiScanʱ��subdiv=2^��p_setting.multiScanPower����ֵ;
			int div;
			for( div=subDiv; ; div/=2)
				if ( ( i & (div-1) )==0 ) break;
			assert(div>=1);
			float gate=p_setting.multGate/subDiv*div*2;
			if (div==subDiv)	//be main line, actually no max gate���ڲ�ʹ��multiScan��ģʽ�����������������
				gate=1e10;
		
			cords.newScan(gate);
			for (int j=0; j<activeList[i].size(); j++)
			{
				CScanEdge &e=activeList[i][j];

				//e�������Աߵ��յ�����ɨ���ߵ����ymaxID��Ҫô���ڵ�ǰɨ����id��ҪôС�ڣ����ᷢ�����ڵ������
				//���ڵ������һ�������Ҫô��ԭʼ�ʹ����ڵ�ǰ�߶�ɨ���߻��Ա������еıߣ�����ʼ�ڵ�ǰɨ���߸߶ȣ���Ҫô�Ǵ�֮ǰ�Ͳ�߶�ɨ�����ϼӽ����ıߣ�
				//���ڵ������ֻ����һ��ʼ�ͱ��ӵ���ǰɨ���߻��Ա������е�ˮƽ�ߣ���ˮƽ��λ�����ɨ�����ϣ�


				if (e.ymaxID<i) //ring line, no clDefault state//����������ǵ�ǰ��λ��ɨ������
				{
					if ( bScanRing ^ cords.bInLine() )	//^ is xor����
						//���1��ѡ��ɨ��ˮƽ�ߣ�������Ҫ��ʼ������ɨ��һ�Գ���㣻	
						//���2����ѡ��ɨ��ˮƽ�ߣ������Ѿ�ɨ����ǰһ���ߵõ���һ����㣬ֻ��ͬʱɨ������ˮƽ�ߵĶ˵���Ϊ�µĳ������һ����㡣
						// (bScanRing && !sords.bInLine) || (!bScanRing && sords.bInLine)
					{
						cords.add(e.x);
						cords.add(e.dx);	//for ring lines, dx is x2 actually //��ʱdx������һ���x���ꣻ
					}
				}
				else //normal cross line
				{
					cords.add(e.x);
				}

				if ((e.ymaxID>i) /*��ǰ���Աߵ��յ��Ը��ڵ�ǰɨ����*/&& ((i+1)<activeList.size()))
				{
					e.x+=e.dx;
					activeList[i+1].add(e);
				}//�����Ա��յ��뵱ǰɨ���ߵǸ�
			}

			activeList[i].free();//��ǰ�߶�ɨ���߽���

			cords.finish(p_lines, p_lines.type(), yStart+i*intver);
		}
	}
}	//end of namespace ZLineScan...

ILineScan& getDefaultILineScan(void)	//head at zsyssevice.h now
{
	using ZLineScanAlgorithm::algorLineScan;
	return algorLineScan;
}

