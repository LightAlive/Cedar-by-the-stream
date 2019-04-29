#include "test2.h"
#include "qfiledialog.h"
#include "QVTKWidget.h"
#include "QDebug.h"
#include<iomanip>
#include "stdlib.h"

class QFileDialog;

test2::test2(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);


	//设置QVTKWidget的渲染器
	m_pRenderer = vtkSmartPointer<vtkRenderer>::New();
	ui.qvtkWidget->GetRenderWindow()->AddRenderer(m_pRenderer);


	actorSTL = vtkSmartPointer<vtkActor>::New();
	actorVoxel = vtkSmartPointer<vtkActor>::New();

	WeldingTorch();
	actorWelding1= vtkSmartPointer<vtkActor>::New();
	actorWelding2 = vtkSmartPointer<vtkActor>::New();
	arr = nullptr;

	//添加坐标系，如非必要可不添加
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	transform->Translate(1.0, 1.0, 1.0);
	vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
	axes->SetUserTransform(transform);
	/*m_pRenderer->AddActor(axes);*/

	//m_pRenderer->SetBackground(1, 1, 1);//把背景色调为白色

	fcube.min.x = 0;
	fcube.max.x = 0;
	fcube.min.y = 0;
	fcube.max.y = 0;
	fcube.min.z = 0;
	fcube.max.z = 0;
	rep = new CZ3DRepresentation(fcube, 0.0f, 0.1f);

}

void test2::on_actionZigzag_triggered()
{
	zigzagDialog = new ZigzagDialog(this);
	zigzagDialog->show();
	zigzagDialog->raise();
	zigzagDialog->activateWindow();
	connect(zigzagDialog, SIGNAL(sendData(double)), this, SLOT(ZigzagButton_clicked()));
}

void test2::on_action_Outline_triggered()
{
	outlineDialog = new OutlineDialog;
	outlineDialog->show();
	outlineDialog->raise();
	outlineDialog->activateWindow();
	connect(outlineDialog, SIGNAL(sendData(double)), this, SLOT(OutlineButton_clicked()));
}


void test2::on_action_Gcode_triggered()
{
	gcodeDialog = new GcodeDialog;
	gcodeDialog->show();
	gcodeDialog->raise();
	gcodeDialog->activateWindow();
	connect(gcodeDialog, SIGNAL(sendData(double)), this, SLOT(GcodeButton_clicked()));
}

void test2::on_action_Robot_triggered()
{
	robotDialog = new RobotDialog;
	robotDialog->show();
	robotDialog->raise();
	robotDialog->activateWindow();

	connect(robotDialog, SIGNAL(sendData(double, int, int, double, double, double, int, int)), this, SLOT(RobotButton_clicked()));
}

void test2::RobotButton_clicked()
{
	//接下来输出机器人文件【先不用这种方式输出】
	QString fileName1 = QFileDialog::getSaveFileName(this, tr("Save RobotFile"), ".", tr("files (*.dat)"));
	QByteArray ba1 = fileName1.toLocal8Bit();
	const char *str1 = ba1.data();
	ofstream SaveFile1(str1);
	
	QString fileName2 = fileName1.replace(QString("dat"), QString("src"));
	QByteArray ba2 = fileName2.toLocal8Bit();
	const char *str2 = ba2.data();
	ofstream SaveFile2(str2);

	robotDATInitDunp(SaveFile1);
	robotSRCInitDunp(SaveFile2);

	double m_a = robotDialog->m_A;
	double m_b = robotDialog->m_B;
	double m_c = robotDialog->m_C;
	int m_s = robotDialog->m_S;
	int m_t = robotDialog->m_T;

	int number = 0;
	for (size_t i = 0; i < m_solidPath.size(); i++)
	{
		for (size_t j = 0; j < m_solidPath.at(i).size(); j++)
		{
			for (size_t k = 0; k < m_solidPath.at(i).at(j).size(); k++)
			{
				SaveFile1 << "DECL E6POS XP" << number << "={x " << rep->toDoubleX(m_solidPath.at(i).at(j).at(k).P().X)
					<< ", y " << rep->toDoubleY(m_solidPath.at(i).at(j).at(k).P().Y)
					<< ", z " << rep->toDoubleZ(m_solidPath.at(i).at(j).at(k).P().P().Z)
					<< ", A " << m_a
					<< ", B " << m_b << ", C " << m_c << ", S " << m_s << ", T " << m_t << ", E1 0.0, E2 0.0, E3 0.0, E4 0.0, E5 0.0, E6 0.0}" << endl;
				SaveFile1 << endl;
				number++;
			}
		}
	}
	SaveFile1 << "DECL BASIS_SUGG_T LAST_BASIS={POINT1[]" << " \"P0 \" " << ",POINT2[]" << " \"P0 \" " << ",CP_PARAMS[]" << " \"CPDAT0 \" " << ",PTP_PARAMS[]" << " \"PDAT0 \" " << ",CONT[] " << " \" \" " << ",CP_VEL[]" << " \"2.0 \" " << ",PTP_VEL[]" << " \"100 \" " << ",SYNC_PARAMS[]" << " \"SYNCDAT \" " << ",SPL_NAME[]" << " \"S0 \" " << ",A_PARAMS[]" << " \"ADAT0 \" " << "}" << endl;
	SaveFile1 << "ENDDAT" << endl;
	SaveFile1.close();
	robotSRCDunp(SaveFile2, number);
	SaveFile2.close();
}

void test2::robotDATInitDunp(ostream &p_os)
{
	//参数初始化
	p_os << "&ACCESS RVP" << '\n' << "&REL 12" << '\n' << "&PARAM DISKPATH = KRC:\\R1" << endl;
	p_os << "DEFDAT  MAINPROGRAM" << '\n' << "EXT  BAS (BAS_COMMAND  :IN,REAL  :IN )" << '\n' << "DECL INT SUCCESS" << endl;
	p_os << endl;

	//坐标系设定
	p_os << "DECL FDAT FP0={TOOL_NO 1,BASE_NO " << robotDialog->m_baseNumber << ",IPO_FRAME #BASE,POINT2[]" << " \" \" " << ",TQ_STATE FALSE}" << endl;//baseNumber基座标号

	//LINE指令相关参数设置
	p_os << "DECL LDAT LCPDAT0={ACC 100.000,APO_DIST 100.000,APO_FAC 50.0000,AXIS_VEL 100.000,AXIS_ACC 100.000,ORI_TYP #VAR,CIRC_TYP #BASE,JERK_FAC 50.0000,GEAR_JERK 50.0000,EXAX_IGN 0}" << endl;

	//PTP指令相关参数
	p_os << "DECL PDAT PPDAT0={ACC 100.000,APO_DIST 100.000,APO_MODE #CPTP,GEAR_JERK 50.0000}" << endl;

	//启弧参数0.00603333377
	p_os << "DECL ATBg_Start_T AS_WDAT0={JobModeId 732750510,ParamSetId -243025295,StartTime 0.200000,PreFlowTime 0.500000,Channel1 1.00000,Channel2 0.0,Channel3 0.0,Channel4 0.0,Channel5 0.0,Channel6 0.0,Channel7 0.0,Channel8 0.0,PurgeTime 0.0}" << endl;	//送气
	p_os << "DECL ATBg_Weld_T AW_WDAT0={JobModeId 732750510,ParamSetId -1540567493,Velocity " << robotDialog->m_speedData << ",Channel1 1.00000,Channel2 0.0,Channel3 0.0,Channel4 0.0,Channel5 0.0,Channel6 0.0,Channel7 0.0,Channel8 0.0}" << endl;//weldSpeed焊接速度
	p_os << "DECL WTCg_WeaveDefinition_T WV_WDAT0={Pattern #None,Length 4.00000,Amplitude 2.00000,Angle 0.0}" << endl;

	//中间过渡参数与启弧参数
	//p_os<<"DECL ATBg_Weld_T AW_WDAT2={JobModeId 732750510,ParamSetId -1540567493,Velocity 0.00603333377,Channel1 1.00000,Channel2 0.0,Channel3 0.0,Channel4 0.0,Channel5 0.0,Channel6 0.0,Channel7 0.0,Channel8 0.0}"<<endl;

	//息弧参数
	p_os << "DECL ATBg_Crater_T AC_WDAT0 = {JobModeId 732750510,ParamSetId 885173050,CraterTime 0.400000,PostflowTime 0.200000,Channel1 1.00000,Channel2 0.0,Channel3 0.0,Channel4 0.0,Channel5 0.0,Channel6 0.0,Channel7 0.0,Channel8 0.0}" << endl;
}

void test2::robotSRCInitDunp(ostream &p_os)
{
	//参数初始化
	p_os << "&ACCESS RVP" << '\n' << "&REL 12" << '\n' << "&PARAM DISKPATH = KRC:\R1" << endl;

	p_os << "DEF MAINPROGRAM()" << '\n';

	p_os << ";FOLD Moduleparameters;%{h}" << endl;
	p_os << ";Params Kuka.MoveDataPtpName=PDAT1; Kuka.VelocityPtp=50; Kuka.PointName=P5; Kuka.BlendingEnabled=True; Kuka.FrameData.tq_state=False; Kuka.FrameData.base_no=8; Kuka.FrameData.tool_no=5; Kuka.FrameData.ipo_frame=#BASE; Kuka.MovementDataPdat.apo_mode=#CPTP; Kuka.MovementDataPdat.apo_dist=100; Kuka.MovementData.acc=100; Kuka.MovementData.gear_jerk=50; ArcTech.WdatVarName=WDAT5; Kuka.MoveDataName=CPDAT4; Kuka.VelocityPath=2; Kuka.MovementData.apo_fac=50; Kuka.MovementData.apo_dist=100; Kuka.MovementData.axis_acc=100; Kuka.MovementData.axis_vel=100; Kuka.MovementData.circ_typ=#BASE; Kuka.MovementData.exax_ign=0; Kuka.MovementData.jerk_fac=50; Kuka.MovementData.ori_typ=#CONSTANT; Kuka.FrameData.point2=" << endl;
	p_os << ";ENDFOLD Moduleparameters" << '\n' << ";FOLD INI;%{PE}" << '\n' << " ;FOLD BASISTECH INI" << endl;

	p_os << "GLOBAL INTERRUPT DECL 3 WHEN $STOPMESS==TRUE DO IR_STOPM ( )" << '\n' << "INTERRUPT ON 3" << '\n' << "BAS (#INITMOV,0 )" << endl;

	p_os << " ;ENDFOLD (BASISTECH INI)" << '\n' << " ;FOLD USER INI" << '\n' << "  ;Make your modifications here\n" << '\n' << " ;ENDFOLD (USER INI)" << '\n' << ";ENDFOLD (INI)" << endl;
	p_os << endl;
}

//如果想在中间起弧熄弧，可以在这个函数给定相应的位置
void test2::robotSRCDunp(ostream &p_os, int p_size)
{
	//回零
	p_os << "$BWDSTART=FALSE" << '\n' << "PDAT_ACT=PDEFAULT" << '\n' << "FDAT_ACT=FHOME" << '\n' << "BAS(#PTP_PARAMS,10)" << '\n' << "$H_POS=XHOME" << '\n' << "PTP XHOME" << endl;
	p_os << endl;

	//	static int j;
	for (int j = 0; j < p_size; ++j)
	{
		//启弧
		if (j == 0)
		{
			p_os << "PDAT_ACT=PPDAT0" << '\n' << "FDAT_ACT=FP0" << '\n' << "BAS(#PTP_PARAMS, 1.0)" << endl;
			p_os << "TRIGGER WHEN DISTANCE = 1 DELAY=ATBg_PreDefinitionTime DO Arc_DefineStrikeParams(1, AS_WDAT0) PRIO = -1" << '\n' << "Arc_DefineCpPattern(#OffInAdvance, WV_WDAT0, TRUE)" << endl;
			p_os << "PTP XP" << j << endl;
			p_os << "Arc_On(1, AS_WDAT0, ATBg_StartErrSetField[1], AW_WDAT0, WV_WDAT0, ATBg_WeldErrSetField[1], #StdArcOn," << " \" \")" << endl;
			p_os << "Arc_DefineCpPattern(#OnInAdvance, WV_WDAT0, TRUE)" << endl;
			p_os << endl;
		}
		//息弧
		else if (j == p_size - 1)
		{
			p_os << "BAS(#CP_PARAMS, ATBg_BAS_VELDefinition)" << endl;
			p_os << "LIN XP" << j << endl;
			p_os << "Arc_Off(1, AC_WDAT0)" << endl;
			p_os << endl;
		}
		// 		//中间焊接过程
		else
		{
			p_os << "LDAT_ACT=LCPDAT0" << '\n' << "LDAT_ACT.APO_Dist = ATBg_APODistanceArcTech" << '\n' << "BAS(#CP_PARAMS, ATBg_BAS_VELDefinition)" << endl;
			p_os << "TRIGGER WHEN DISTANCE = 1 DELAY = 0 DO Arc_Swi(1, AW_WDAT0, WV_WDAT0, ATBg_WeldErrSetField[1]) PRIO = -1" << endl;
			p_os << "LIN XP" << j << " C_Dis C_Dis" << endl;
			p_os << "ATB_Definition(AW_WDAT0)" << endl;
			p_os << endl;
		}
	}
	//回零
	p_os << "$BWDSTART=FALSE" << '\n' << "PDAT_ACT=PDEFAULT" << '\n' << "FDAT_ACT=FHOME" << '\n' << "BAS(#PTP_PARAMS,10)" << '\n' << "$H_POS=XHOME" << '\n' << "PTP XHOME" << endl;
	p_os << "END" << endl;
	p_os << endl;
}


void test2::GcodeButton_clicked()
{
	//接下来G代码[试试双道焊]
	QString fileName1 = QFileDialog::getSaveFileName(this, tr("Save GcodeFile"), ".", tr("files (*.nc)"));
	QByteArray ba1 = fileName1.toLocal8Bit();
	const char *str1 = ba1.data();
	ofstream SaveFile1(str1);

	double m_speedParameter = gcodeDialog->speedParameter;
	double x, y, z;
	x = -7.2;
	y = -50;
	z = -14;

	//SaveFile1 << "M15" << endl;
	SaveFile1 << "G90 G54" /*<< " G00 X0 Y0 Z150 F700.0" << endl*/;
	SaveFile1 << " G00 X" << std::fixed <<setprecision(3)<<m_solidPath.at(0).at(0).at(0).P().X + x << " Y" << std::fixed << setprecision(3) << m_solidPath.at(0).at(0).at(0).P().Y + y << " Z" << 0 << " F700.0" << endl;
	SaveFile1 << " G00 X" << std::fixed << setprecision(3) << m_solidPath.at(0).at(0).at(0).P().X+x << " Y" << std::fixed << setprecision(3) << m_solidPath.at(0).at(0).at(0).P().Y+y << " Z" << std::fixed << setprecision(3) << m_solidPath.at(0).at(0).at(0).P().Z+z << " F700.0" << endl;

	SaveFile1 << "M17" << endl;
	for (size_t i = 0; i < m_solidPath.size(); i++)
	{
		
		SaveFile1 << "N00" << i <<endl;
		for (size_t j = 0; j < m_solidPath.at(i).size(); j++)
		{
			for (size_t k = 0; k < m_solidPath.at(i).at(j).size(); k++)
			{
				SaveFile1 << "G01 X" << std::fixed << setprecision(3) << m_solidPath.at(i).at(j).at(k).P().X+x
					<< " Y" << std::fixed << setprecision(3) << m_solidPath.at(i).at(j).at(k).P().Y+y
					<< " Z" << std::fixed << setprecision(3)<< m_solidPath.at(i).at(j).at(k).P().Z+z;
				SaveFile1 << endl;
			}
			
		}
		//SaveFile1 << "G00 X" << m_solidPath.at(i).at(j).at(k).P().X + x
		
	}
	
	SaveFile1 << "M18" << endl;
	SaveFile1 << "M30";
	SaveFile1.close();
}

void test2::on_action_Tool_triggered()
{
	toolLayerDialog = new ToolLayerDialog;
	toolLayerDialog->show();
	toolLayerDialog->raise();
	toolLayerDialog->activateWindow();

	connect(toolLayerDialog, SIGNAL(sendData(double)), this, SLOT(ToolLayerButton_clicked()));
}

void test2::on_action_Voxel_triggered()
{
	voxelizationDialog = new VoxelizatiponDialog;
	voxelizationDialog->show();
	voxelizationDialog->raise();
	voxelizationDialog->activateWindow();
	connect(voxelizationDialog, SIGNAL(sendData(double)), this, SLOT(voxelizationButten_clicked()));
}
void test2::on_actionDetection_triggered()
{

	actorWelding1->GetProperty()->SetColor(0, 0.5, 0);
	actorWelding2->GetProperty()->SetColor(0, 0.5, 0);
	m_pRenderer->AddActor(actorWelding1);
	m_pRenderer->AddActor(actorWelding2);

	Byte *a = rep->getBuff();
	int m_yCoefficient ,my,mz,maxz,m_zCoefficient;
	my = rep->getMaxX(); mz = rep->getMaxY(); maxz = rep->getMaxZ();
	m_yCoefficient = my + 1; m_zCoefficient = (mz + 1)*m_yCoefficient;
	
	
	unsigned long long  sum = ((unsigned long long)my + 1)*
		((unsigned long long)mz + 1)*((unsigned long long)maxz + 1);
	double  x, y, z, xm, ym, zm,p; int i = 0, j = 0; string str;
	xm = reader->GetOutput()->GetBounds()[0];
	ym = reader->GetOutput()->GetBounds()[2];
	zm = reader->GetOutput()->GetBounds()[4];
	p = rep->getRessolution();
	ifstream infile;
	infile.open("D:\\VS2015-practice\\QtGuiApplication1\\QtGuiApplication1\\p.txt");
	int index = 0;
	while (!infile.eof()) {
		for (i = 0; i < 3; i++) {
			infile >> str;
			if (str[0] >= '0'&&str[0] <= '9' || str[0] == '-') {
				//圆柱模型测试
				if (i == 0) {
					x = atof(str.c_str())-16.8;
				}
				if (i == 1) {
					y = atof(str.c_str())+25;
				}
				if (i == 2) {
					z = atof(str.c_str())+44;
				}
				//模具测试
				/*if (i == 0) {
					x = atof(str.c_str());
				}
				if (i == 1) {
					y = atof(str.c_str());
				}
				if (i == 2) {
					z = atof(str.c_str())+250;
				}*/
			}
			else i--;
			if (infile.eof())break;
		}
		double xx = x, yy = y, zz = z;cout << x << ' ' << y << ' ' << z << ',';
		x -= xm;
		y -= ym; 
		z -= zm; 
		x = rep->toInt(x);
		y = rep->toInt(y);
		z = rep->toInt(z);
        
		weld.setx(x); weld.sety(y); weld.setz(z);
		weld.setlx(0); weld.setly(0);
		if (!weld.Detection(a, m_yCoefficient, m_zCoefficient, sum))
		weld.Dispose(a, m_yCoefficient, m_zCoefficient, sum);cout << weld.getlx() << ' ' << weld.getly()  << endl;
		
		weld.setx(xx); weld.sety(yy); weld.setz(zz);
		actorWelding1->SetMapper(weld.draw1());
		actorWelding2->SetMapper(weld.draw2());
		ui.qvtkWidget->GetRenderWindow()->Render();
		
		/*arr[index][0] = xx; arr[index][1] = yy; arr[index][2] = zz; arr[index][3] = weld.getlx(); arr[index][4] = weld.getly(); 
		index++;*/
		Sleep(10);
	}
	infile.close();
	ui.qvtkWidget->GetRenderWindow()->Render();
}
void test2::on_actionRobot_triggered()         //生成机器人.src文件
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save RobotSrcFile"), ".", tr("files (*.src)"));
	QByteArray ba = fileName.toLocal8Bit();
	const char *str = ba.data();
	ofstream SaveFile(str);

	SaveFile << "&ACCESS RVP" << endl; SaveFile << "&PARAM DISKPATH = KRC:\\R1\\Program" << endl;
	SaveFile << "DEF testExample( )" << endl;
	SaveFile << ";FOLD INI;%{PE}\n"
		"; FOLD BASISTECH INI\n"
		"  GLOBAL INTERRUPT DECL 3 WHEN $STOPMESS == TRUE DO IR_STOPM ( )\n"
		"INTERRUPT ON 3\n"
		"BAS(#INITMOV, 0)\n"
		"; ENDFOLD(BASISTECH INI)\n"
		"; FOLD USER INI\n"
		"; Make your modifications here\n\n"
		"; ENDFOLD(USER INI)\n"
		"; ENDFOLD(INI)\n";
	for (int i = 0; i < 8; i++) {
		SaveFile << ";FOLD PTP P" << i << " Vel=10 % PDAT4 Tool[1]:tool1 Base[0];%{PE}\n"
			"PDAT_ACT=PPDAT"<<i<<"\n"
			"FDAT_ACT = FP" << i << "\n"
			"BAS(#PTP_PARAMS, 10)\n"
			"PTP XP" << i << "\n"
			"; ENDFOLD\n";
	}

	SaveFile << "END" << endl;
	SaveFile.close();
}
void test2::on_action_dat_triggered()         //生成机器人.dat文件
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save RobotDatFile"), ".", tr("files (*.dat)"));
	QByteArray ba = fileName.toLocal8Bit();
	const char *str = ba.data();
	ofstream SaveFile(str);

	SaveFile << "&ACCESS RVP\n""&PARAM DISKPATH = KRC:\\R1\\Program\n"
		"DEFDAT  testExample\n"
		";FOLD EXTERNAL DECLARATIONS;%{PE}%MKUKATPBASIS,%CEXT,%VCOMMON,%P\n"
		"; FOLD BASISTECH EXT; % {PE}%MKUKATPBASIS, %CEXT, %VEXT, %P\n"
		"EXT  BAS(BAS_COMMAND  :IN, REAL : IN)\n"
		"DECL INT SUCCESS\n"
		"; ENDFOLD(BASISTECH EXT)\n"
		"; FOLD USER EXT; % {E}%MKUKATPUSER, %CEXT, %VEXT, %P\n"
		";ENDFOLD (USER EXT)\n"
		"; ENDFOLD(EXTERNAL DECLARATIONS)\n";
	SaveFile << "DECL BASIS_SUGG_T LAST_BASIS={POINT1[]" "P11                     "",POINT2[] ""P11                     "",CP_PARAMS[] ""CPDAT1                  "",PTP_PARAMS[] ""PDAT7                   "",CONT[] "                        ",CP_VEL[] ""2                       "",PTP_VEL[] ""10                      "",SYNC_PARAMS[] ""SYNCDAT                 "",SPL_NAME[] ""S0                      "",A_PARAMS[] ""ADAT0                   ""}\n"
		"DECL E6POS XP10 = { X 0,Y 0,Z 30,A 0,B 0,C 0,S 18,T 34,E1 0.0,E2 0.0,E3 0.0,E4 0.0,E5 0.0,E6 0.0 }"
		"DECL FDAT FP10 = { TOOL_NO 1,BASE_NO 0,IPO_FRAME #BASE,POINT2[] " ",TQ_STATE FALSE }\n ";
	double pat[8][5] = { -22,7, 32,0.1309, 0.1309 ,
		-12, - 20, 32,-0.19635, 0.19635,
		8, - 22.5, 32,-0.261799, - 0.19635,
		20, - 10.5, 32,-0.0654498 ,- 0.0654498,
		20, - 10.5, 30.5,-0.0654498, - 0.0654498,
		8, - 22.5, 30.5,-0.261799, - 0.19635,
		-12, - 20, 30.5,-0.19635, 0.19635,
		-22, 7, 30.5,0.1309, 0.1309 };
	for (int i = 0; i < 8; i++) {
		SaveFile << "DECL E6POS XP" << i << "={X "<<pat[i][0]<<",Y"<< pat[i][1] << ",Z " << pat[i][2] << ",A " << 0 << ",B " << pat[i][4] << ",C "<<pat[i][3]<<",S 18,T 2,E1 0.0,E2 0.0,E3 0.0,E4 0.0,E5 0.0,E6 0.0}\n"
			"DECL FDAT FP" << i << " = { TOOL_NO 1,BASE_NO 0,IPO_FRAME #BASE,POINT2[] " ",TQ_STATE FALSE }\n"
			"DECL PDAT PPDAT" << i << " = { VEL 100.000,ACC 100.000,APO_DIST 100.000,GEAR_JERK 50.0000,EXAX_IGN 0 }\n";
	}


	SaveFile << "ENDDAT" << endl;
	SaveFile.close();
}

void test2::on_action_STL_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open STL"), ".",
		tr("files (*.stl )"));

	//支持带中文路径的读取
	const QByteArray text = fileName.toLocal8Bit();
	const char *textdata = text.data();
	reader = vtkSmartPointer<vtkSTLReader>::New();
	reader->SetFileName(textdata);
	reader->Update();
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(reader->GetOutputPort());
	actorSTL->SetMapper(mapper);
	//actorSTL->GetProperty()->SetColor(0, 0, 0);//给打开的STL添加颜色
	m_pRenderer->AddActor(actorSTL);
	ui.qvtkWidget->GetRenderWindow()->Render();
}

void test2::on_action_Simulation_triggered()
{
	m_pRenderer->RemoveAllViewProps();
	vtkSmartPointer<vtkActor> weldActor1 = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> weldActor2 = vtkSmartPointer<vtkActor>::New();
	
	//绘制实体STL文件
	//打开STL文件,显示STL文件..STL要移动位置
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open STL"), ".",
		tr("files (*.stl )"));

	//支持带中文路径的读取
	const QByteArray text = fileName.toLocal8Bit();
	const char *textdata = text.data();
	vtkSmartPointer<vtkSTLReader> reader1 = vtkSmartPointer<vtkSTLReader>::New();
	reader1 = vtkSmartPointer<vtkSTLReader>::New();
	reader1->SetFileName(textdata);
	reader1->Update();

	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transform->Translate(-1 * reader->GetOutput()->GetBounds()[0], -1 * reader->GetOutput()->GetBounds()[2], -1 * reader->GetOutput()->GetBounds()[4]);
	transformFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(transformFilter->GetOutputPort());
	vtkSmartPointer<vtkActor> actorSTL1 = vtkSmartPointer<vtkActor>::New();
	actorSTL1->SetMapper(mapper);
	//actorSTL1->GetProperty()->SetColor(0, 0, 0);//给打开的STL添加颜色
	m_pRenderer->AddActor(actorSTL1);
	ui.qvtkWidget->GetRenderWindow()->Render();
	
	
	
	for (size_t i = 0; i < m_solidPath.size(); i++)
	{
		for (size_t j = 0; j < m_solidPath.at(i).size(); j++)
		{
			//找到每条曲线中的起止点对
			vector<int> startPoint;
			vector<int> endPoint;
			for (size_t k = 0; k < m_solidPath.at(i).at(j).size(); k++)
			{
				if ((int)m_solidPath.at(i).at(j).at(k).P().A == 1)
				{
					startPoint.push_back((int)k);
				}
				else if ((int)m_solidPath.at(i).at(j).at(k).P().A == 2)
				{
					endPoint.push_back((int)k);
				}
			}

			if (startPoint.size() == 0)
			{
				break;
			}

			//从起点到终点绘制
			if (startPoint.size() > 0)
			{
				if (startPoint.at(0) < endPoint.at(0))//Y正向扫描的路径
				{
					for (int p = 0; p < startPoint.size(); p++)
					{
						for (int q = startPoint.at(p)+1; q <=endPoint.at(p); q++)
						{
							vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
							for (vtkIdType l = startPoint.at(p); l <= q; l++)
							{
								points->InsertPoint(l, m_solidPath.at(i).at(j).at(l).P().X, m_solidPath.at(i).at(j).at(l).P().Y, m_solidPath.at(i).at(j).at(l).P().Z);
							}
							//绘制图形
							vtkSmartPointer<vtkParametricSpline> spline = vtkSmartPointer<vtkParametricSpline>::New();
							spline->SetPoints(points);
							spline->ClosedOff();

							vtkSmartPointer<vtkParametricFunctionSource> splineSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
							splineSource->SetParametricFunction(spline);
							splineSource->Update();

							vtkSmartPointer<vtkTubeFilter> tube = vtkSmartPointer<vtkTubeFilter>::New();
							tube->SetInputData(splineSource->GetOutput());
							tube->SetRadius(2.0);//半径大小可根据要求改变
							tube->SetNumberOfSides(200);
							tube->Update();
							vtkSmartPointer<vtkPolyDataMapper> tubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
							tubeMapper->SetInputConnection(tube->GetOutputPort());

							//vtkSmartPointer<vtkPNGReader> readerPNG = vtkSmartPointer<vtkPNGReader>::New();
							vtkSmartPointer<vtkJPEGReader> readerPNG = vtkSmartPointer<vtkJPEGReader>::New();
							readerPNG->SetFileName("..\\wenli.jpg");
							vtkSmartPointer<vtkTexture> texture = vtkSmartPointer<vtkTexture>::New();
							texture->SetInputConnection(readerPNG->GetOutputPort());
							texture->InterpolateOn();

							vtkSmartPointer<vtkActor> weldActor = vtkSmartPointer<vtkActor>::New();
							weldActor->SetMapper(tubeMapper);
							weldActor->GetProperty()->SetColor(/*double(252.0 / 255.0), double(157.0 / 255.0), double(154.0 / 255.0)*/ 1, 0, 1);
							m_pRenderer->AddActor(weldActor);

							//加焊枪
							vtkSmartPointer<vtkLineSource> lineWeld = vtkSmartPointer<vtkLineSource>::New();
							lineWeld->SetPoint1(m_solidPath.at(i).at(j).at(q).P().X, m_solidPath.at(i).at(j).at(q).P().Y, m_solidPath.at(i).at(j).at(q).P().Z + 5);
							lineWeld->SetPoint2(m_solidPath.at(i).at(j).at(q).P().X, m_solidPath.at(i).at(j).at(q).P().Y, m_solidPath.at(i).at(j).at(q).P().Z + 15);
							lineWeld->Update();

							vtkSmartPointer<vtkTubeFilter> tubeFilterWeld = vtkSmartPointer<vtkTubeFilter>::New();
							tubeFilterWeld->SetInputConnection(lineWeld->GetOutputPort());
							tubeFilterWeld->SetRadius(1.0);
							tubeFilterWeld->SetNumberOfSides(100);
							tubeFilterWeld->CappingOn();
							tubeFilterWeld->Update();

							vtkSmartPointer<vtkPolyDataMapper> tubeMapperWeld = vtkSmartPointer<vtkPolyDataMapper>::New();
							tubeMapperWeld->SetInputConnection(tubeFilterWeld->GetOutputPort());
							
							weldActor1->SetMapper(tubeMapperWeld);
							weldActor1->GetProperty()->SetColor(1, 1, 0);
							weldActor1->GetProperty()->SetSpecular(0.4);
							weldActor1->GetProperty()->SetShading(10);

							//接下来再加一个圆柱体
							vtkSmartPointer<vtkLineSource> lineWeld2 = vtkSmartPointer<vtkLineSource>::New();
							lineWeld2->SetPoint1(m_solidPath.at(i).at(j).at(q).P().X, m_solidPath.at(i).at(j).at(q).P().Y, m_solidPath.at(i).at(j).at(q).P().Z + 15);
							lineWeld2->SetPoint2(m_solidPath.at(i).at(j).at(q).P().X, m_solidPath.at(i).at(j).at(q).P().Y, m_solidPath.at(i).at(j).at(q).P().Z + 20);
							lineWeld2->Update();

							vtkSmartPointer<vtkTubeFilter> tubeFilterWeld2 = vtkSmartPointer<vtkTubeFilter>::New();
							tubeFilterWeld2->SetInputConnection(lineWeld2->GetOutputPort());
							tubeFilterWeld2->SetRadius(5.0);
							tubeFilterWeld2->SetNumberOfSides(100);
							tubeFilterWeld2->CappingOn();
							tubeFilterWeld2->Update();

							vtkSmartPointer<vtkPolyDataMapper> tubeMapperWeld2 = vtkSmartPointer<vtkPolyDataMapper>::New();
							tubeMapperWeld2->SetInputConnection(tubeFilterWeld2->GetOutputPort());
							weldActor2->SetMapper(tubeMapperWeld2);
							weldActor2->GetProperty()->SetColor(1, 1, 0);
							weldActor2->GetProperty()->SetSpecular(0.4);
							weldActor2->GetProperty()->SetShading(10);

							/*m_pRenderer->AddActor(weldActor1);
							m_pRenderer->AddActor(weldActor2);
							ui.qvtkWidget->GetRenderWindow()->Render();*/
						}
					}
				}

				else if (startPoint.at(0) > endPoint.at(0))//Y负向扫描的路径
				{
					for (int p = 0; p < startPoint.size(); p++)
					{
						for (int q = startPoint.at(p) - 1; q >= endPoint.at(p); q--)
						{
							vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
							for (vtkIdType l = startPoint.at(p); l >= q; l--)
							{
								points->InsertPoint(l, m_solidPath.at(i).at(j).at(l).P().X, m_solidPath.at(i).at(j).at(l).P().Y, m_solidPath.at(i).at(j).at(l).P().Z);
							}
							//绘制图形
							vtkSmartPointer<vtkParametricSpline> spline = vtkSmartPointer<vtkParametricSpline>::New();
							spline->SetPoints(points);
							spline->ClosedOff();

							vtkSmartPointer<vtkParametricFunctionSource> splineSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
							splineSource->SetParametricFunction(spline);
							splineSource->Update();

							vtkSmartPointer<vtkTubeFilter> tube = vtkSmartPointer<vtkTubeFilter>::New();
							tube->SetInputData(splineSource->GetOutput());
							tube->SetRadius(2.0);//半径大小可根据要求改变
							tube->SetNumberOfSides(200);
							tube->Update();
							vtkSmartPointer<vtkPolyDataMapper> tubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
							tubeMapper->SetInputConnection(tube->GetOutputPort());

							//vtkSmartPointer<vtkPNGReader> readerPNG = vtkSmartPointer<vtkPNGReader>::New();
							vtkSmartPointer<vtkJPEGReader> readerPNG = vtkSmartPointer<vtkJPEGReader>::New();
							readerPNG->SetFileName("..\\wenli.jpg");
							vtkSmartPointer<vtkTexture> texture = vtkSmartPointer<vtkTexture>::New();
							texture->SetInputConnection(readerPNG->GetOutputPort());
							texture->InterpolateOn();

							vtkSmartPointer<vtkActor> weldActor = vtkSmartPointer<vtkActor>::New();
							weldActor->SetMapper(tubeMapper);
							weldActor->GetProperty()->SetColor(double(252.0 / 255.0), double(157.0 / 255.0), double(154.0 / 255.0));
							m_pRenderer->AddActor(weldActor);
							
							//加焊枪
							vtkSmartPointer<vtkLineSource> lineWeld = vtkSmartPointer<vtkLineSource>::New();
							lineWeld->SetPoint1(m_solidPath.at(i).at(j).at(q).P().X, m_solidPath.at(i).at(j).at(q).P().Y, m_solidPath.at(i).at(j).at(q).P().Z + 5);
							lineWeld->SetPoint2(m_solidPath.at(i).at(j).at(q).P().X, m_solidPath.at(i).at(j).at(q).P().Y, m_solidPath.at(i).at(j).at(q).P().Z + 15);
							lineWeld->Update();

							vtkSmartPointer<vtkTubeFilter> tubeFilterWeld = vtkSmartPointer<vtkTubeFilter>::New();
							tubeFilterWeld->SetInputConnection(lineWeld->GetOutputPort());
							tubeFilterWeld->SetRadius(1.0);
							tubeFilterWeld->SetNumberOfSides(100);
							tubeFilterWeld->CappingOn();
							tubeFilterWeld->Update();

							vtkSmartPointer<vtkPolyDataMapper> tubeMapperWeld = vtkSmartPointer<vtkPolyDataMapper>::New();
							tubeMapperWeld->SetInputConnection(tubeFilterWeld->GetOutputPort());

							weldActor1->SetMapper(tubeMapperWeld);
							weldActor1->GetProperty()->SetColor(1, 1, 0);
							weldActor1->GetProperty()->SetSpecular(0.4);
							weldActor1->GetProperty()->SetShading(10);

							//接下来再加一个圆柱体
							vtkSmartPointer<vtkLineSource> lineWeld2 = vtkSmartPointer<vtkLineSource>::New();
							lineWeld2->SetPoint1(m_solidPath.at(i).at(j).at(q).P().X, m_solidPath.at(i).at(j).at(q).P().Y, m_solidPath.at(i).at(j).at(q).P().Z + 15);
							lineWeld2->SetPoint2(m_solidPath.at(i).at(j).at(q).P().X, m_solidPath.at(i).at(j).at(q).P().Y, m_solidPath.at(i).at(j).at(q).P().Z + 20);
							lineWeld2->Update();

							vtkSmartPointer<vtkTubeFilter> tubeFilterWeld2 = vtkSmartPointer<vtkTubeFilter>::New();
							tubeFilterWeld2->SetInputConnection(lineWeld2->GetOutputPort());
							tubeFilterWeld2->SetRadius(5.0);
							tubeFilterWeld2->SetNumberOfSides(100);
							tubeFilterWeld2->CappingOn();
							tubeFilterWeld2->Update();

							vtkSmartPointer<vtkPolyDataMapper> tubeMapperWeld2 = vtkSmartPointer<vtkPolyDataMapper>::New();
							tubeMapperWeld2->SetInputConnection(tubeFilterWeld2->GetOutputPort());
							weldActor2->SetMapper(tubeMapperWeld2);
							weldActor2->GetProperty()->SetColor(1, 1, 0);
							weldActor2->GetProperty()->SetSpecular(0.4);
							weldActor2->GetProperty()->SetShading(10);

							m_pRenderer->AddActor(weldActor1);
							m_pRenderer->AddActor(weldActor2);

							//显示实体[结果显示不出来]
							vtkSmartPointer<vtkSTLReader> solidReader = vtkSmartPointer<vtkSTLReader>::New();
							solidReader->SetFileName("..\\solid10.stl");
							solidReader->Update();

							vtkSmartPointer<vtkPolyDataMapper> solidMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
							solidMapper->SetInputConnection(solidReader->GetOutputPort());
							vtkSmartPointer<vtkActor> solidactor = vtkSmartPointer<vtkActor>::New();
							solidactor->SetMapper(solidMapper);
							//solidactor->GetProperty()->SetColor(0, 0, 0);//给打开的STL添加颜色
							m_pRenderer->AddActor(solidactor);
							ui.qvtkWidget->GetRenderWindow()->Render();
							//结束显示实体***************


							ui.qvtkWidget->GetRenderWindow()->Render();

						}
					}
				}

				startPoint.clear();
				endPoint.clear();
			}

		}
	}



}

void test2::on_action_IsochoricLayer_triggered()
{
	growthLineLayerDialog = new GrowthLineLayerDialog;
	growthLineLayerDialog->show();
	growthLineLayerDialog->raise();
	growthLineLayerDialog->activateWindow();
	connect(growthLineLayerDialog, SIGNAL(sendData(double)), this, SLOT(GrowthLineIsochoricLayerButton_clicked()));
}

void test2::GrowthLineIsochoricLayerButton_clicked()
{
	double layerHigh = growthLineLayerDialog->dOfLayer;
	rep->makeGrowthLineCurveLayer(layerHigh, buff_curvedLayer);
	
	//体素显示
	m_pRenderer->RemoveActor(actorVoxel);//清除已有的actor
	vector<vtkActor * > curvedActor(buff_curvedLayer.size());
	for (int i = 0; i < buff_curvedLayer.size(); i++)
	{
		curvedActor.at(i) = vtkActor::New();
	}
	for (int i = 0; i < buff_curvedLayer.size(); i++)
	{
		vtkSmartPointer<vtkPoints> ugpoints = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
		vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
		glDrawCurve(buff_curvedLayer.at(i), ugpoints, ug, cells);
		vtkSmartPointer<vtkDataSetMapper> ugmapper1 = vtkSmartPointer<vtkDataSetMapper>::New();
		ugmapper1->SetInputData(ug);
		curvedActor.at(i)->SetMapper(ugmapper1);

		if (i % 2 == 0)
		{
			curvedActor.at(i)->GetProperty()->SetColor(1, 0, 0);
		}
		else
		{
			curvedActor.at(i)->GetProperty()->SetColor(0, 1, 0);
		}
		//curvedActor.at(i)->GetProperty()->SetColor(1, 1, 1/*double(220.0/255.0), double(230.0/255.0), double(240.0/255.0)*/);
		m_pRenderer->AddActor(curvedActor.at(i));
		ui.qvtkWidget->GetRenderWindow()->Render();
	}
}

void test2::on_action_VariableThicknessLayer_triggered()
{
	growthLineVariousLayerDialog = new GrowthLineVariousLayerDialog;
	growthLineVariousLayerDialog->show();
	growthLineVariousLayerDialog->raise();
	growthLineVariousLayerDialog->activateWindow();
	connect(growthLineVariousLayerDialog, SIGNAL(sendData(double, double)), this, SLOT(GrowthLineVariousLayerButton_clicked()));
}

void test2::GrowthLineVariousLayerButton_clicked()
{
	double layerMaxHigh = growthLineVariousLayerDialog->dOfMaxLayer;
	double layerMinHigh = growthLineVariousLayerDialog->dOfMinLayer;
	rep->makeGrowthLineVirousCurveLayer(layerMinHigh, layerMaxHigh, buff_curvedLayer);

	//体素显示
	m_pRenderer->RemoveActor(actorVoxel);//清除已有的actor
	vector<vtkActor * > curvedActor(buff_curvedLayer.size());
	for (int i = 0; i < buff_curvedLayer.size(); i++)
	{
		curvedActor.at(i) = vtkActor::New();
	}
	for (int i = 0; i < buff_curvedLayer.size(); i++)
	{
		vtkSmartPointer<vtkPoints> ugpoints = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
		vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
		glDrawCurve(buff_curvedLayer.at(i), ugpoints, ug, cells);
		vtkSmartPointer<vtkDataSetMapper> ugmapper1 = vtkSmartPointer<vtkDataSetMapper>::New();
		ugmapper1->SetInputData(ug);
		curvedActor.at(i)->SetMapper(ugmapper1);

		if (i % 2 == 0)
		{
			curvedActor.at(i)->GetProperty()->SetColor(1, 0, 0);
		}
		else
		{
			curvedActor.at(i)->GetProperty()->SetColor(0, 1, 0);
		}
		//curvedActor.at(i)->GetProperty()->SetColor(1, 1, 1/*double(220.0/255.0), double(230.0/255.0), double(240.0/255.0)*/);
		m_pRenderer->AddActor(curvedActor.at(i));
		ui.qvtkWidget->GetRenderWindow()->Render();
	}
}

void test2::voxelizationButten_clicked()
{
	//先清除已有的actor
	m_pRenderer->RemoveActor(actorSTL);
	m_pRenderer->RemoveActor(actorVoxel);
	m_pRenderer->RemoveActor(actorWelding1);
	m_pRenderer->RemoveActor(actorWelding2);

	double voxelizationPrecision = voxelizationDialog->parameter;//得到体素化精度的参数
	//vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transform->Translate(-1 * reader->GetOutput()->GetBounds()[0], -1 * reader->GetOutput()->GetBounds()[2], -1 * reader->GetOutput()->GetBounds()[4]);
	transformFilter->SetInputData(reader->GetOutput());
	transformFilter->SetTransform(transform);
	transformFilter->Update();
	vtkSmartPointer<vtkPolyData> stl = vtkSmartPointer<vtkPolyData>::New();
	stl = transformFilter->GetOutput();
	//cube->SetBounds(stl->GetBounds());
	//cube->SetCenter(stl->GetCenter());
	CCubef &fcube1 = fcube;//创建一个float类型的cube
	fcube1.min.x = stl->GetBounds()[0];
	fcube1.max.x = stl->GetBounds()[1];
	fcube1.min.y = stl->GetBounds()[2];
	fcube1.max.y = stl->GetBounds()[3];
	fcube1.min.z = stl->GetBounds()[4];
	fcube1.max.z = stl->GetBounds()[5];
	fcube1.max = fcube1.max - fcube1.min;
	fcube1.min = fcube1.min - fcube1.min;
	CPoint3d Center_STL;//STL实体的中心点（就是cube的中心点）,CPoint3d类型是一个三维的double类型的点。
	Center_STL.x = fcube.midX();
	Center_STL.y = fcube.midY();
	Center_STL.z = fcube.midZ();
	
	rep = new CZ3DRepresentation(fcube1, 0.0f, voxelizationPrecision);
	rep->makeSolid(stl, 0x01, 0x01);
	vtkSmartPointer<vtkPoints> ugpoints = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
	vtkSmartPointer<vtkCellArray> cells =vtkSmartPointer<vtkCellArray>::New();
	//rep->glDraw(ugpoints, ug, cells);
	//调用如下重载函数将stl实际坐标和体素坐标重合
	rep->glDraw(ugpoints, ug, cells, reader->GetOutput()->GetBounds()[0]- voxelizationPrecision, 
		reader->GetOutput()->GetBounds()[2] - voxelizationPrecision, reader->GetOutput()->GetBounds()[4] - voxelizationPrecision);

	vtkSmartPointer<vtkDataSetMapper> ugmapper1 = vtkSmartPointer<vtkDataSetMapper>::New();
	ugmapper1->SetInputData(ug);
	actorVoxel->SetMapper(ugmapper1);
	actorVoxel->GetProperty()->SetColor(1,1,1/*double(220.0/255.0), double(230.0/255.0), double(240.0/255.0)*/);
	m_pRenderer->AddActor(actorVoxel);

	ui.qvtkWidget->GetRenderWindow()->Render();
}

void test2::ToolLayerButton_clicked()
{
	double layerHeigh = toolLayerDialog->dOfLayer;
	makeCurvedLayer(layerHeigh);
}

void test2::makeCurvedLayer(double m_layerHigh)
{
	//rep->makeTopCurvedLayer();//获取最上层曲面作为刀具曲面
	rep->makeBottomCurvedLayer();//获取最底层曲面作为刀具曲面
	
							  
	//获取圆管面做的刀具曲面****************
	//思路是：从基底的STL文件中获取刀具曲面，而非从自身STL模型中获取刀具曲面
	
	//首先打开一个STL文件，将其体素化，用上一个刀具曲面函数获取其数据
	//vtkSmartPointer<vtkSTLReader> reader1 = vtkSmartPointer<vtkSTLReader>::New();
	//reader1->SetFileName("..\\YuanZhu.stl");//读取圆管STL文件
	//reader1->Update();
	//
	//vtkSmartPointer<vtkTransform> transform1 = vtkSmartPointer<vtkTransform>::New();//将圆管移动到以原点为起点的第一象限
	//transform1->Translate(-1 * reader1->GetOutput()->GetBounds()[0], -1 * reader1->GetOutput()->GetBounds()[2], -1 * reader1->GetOutput()->GetBounds()[4]);

	//vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter1 = vtkSmartPointer<vtkTransformPolyDataFilter>::New();//VTK的Filter，用于处理读入的数据[将STL执行坐标转换]
	//transformFilter1->SetInputData(reader1->GetOutput());
	//transformFilter1->SetTransform(transform1);
	//transformFilter1->Update();
	//
	//vtkSmartPointer<vtkPolyData> stl1 = vtkSmartPointer<vtkPolyData>::New();//此时的STL为坐标变换之后的STL
	//stl1 = transformFilter1->GetOutput();
	//
	//vtkSmartPointer<vtkCubeSource> cube1 = vtkSmartPointer<vtkCubeSource>::New();//构建包围盒
	//cube1->SetBounds(stl1->GetBounds());
	//cube1->SetCenter(stl1->GetCenter());
	//
	//CCubef fcube2;//创建一个float类型的cube
	//fcube2.min.x = stl1->GetBounds()[0];
	//fcube2.max.x = stl1->GetBounds()[1];
	//fcube2.min.y = stl1->GetBounds()[2];
	//fcube2.max.y = stl1->GetBounds()[3];
	//fcube2.min.z = stl1->GetBounds()[4];
	//fcube2.max.z = stl1->GetBounds()[5];
	//fcube2.max = fcube2.max - fcube2.min;
	//fcube2.min = fcube2.min - fcube2.min;
	//CPoint3d Center_STL1;//STL实体的中心点（就是cube的中心点）,CPoint3d类型是一个三维的double类型的点。
	//Center_STL1.x = fcube2.midX();
	//Center_STL1.y = fcube2.midY();
	//Center_STL1.z = fcube2.midZ();

	//double voxelizationPrecision = voxelizationDialog->parameter;//得到体素化精度的参数
	//CZ3DRepresentation *rep1;
	//rep1 = new CZ3DRepresentation(fcube2, 0.0f, voxelizationPrecision);
	//rep1->makeSolid(stl1, 0x01, 0x01);
	//rep1->makeTopCurvedLayer();
	//
	///*int m_yCoefficient = rep1->getMaxX() + 1;
	//int m_zCoefficient = (rep1->getMaxY() + 1)*(rep1->getMaxX() + 1);
	//int n = rep1->getMaxZ()*m_zCoefficient + rep1->getMaxY()*m_yCoefficient + rep1->getMaxX();*/
	//
	////rep->makeTopCurvedLayer();
	//rep->makeTopCurvedLayer2(rep1->m_buff_CurvedLayer,(reader1->GetOutput()->GetBounds()[0] - reader->GetOutput()->GetBounds()[0])/voxelizationPrecision, (reader1->GetOutput()->GetBounds()[2] - reader->GetOutput()->GetBounds()[2]) / voxelizationPrecision, (reader1->GetOutput()->GetBounds()[4] - reader->GetOutput()->GetBounds()[4]) / voxelizationPrecision, rep1->getMaxX(), rep1->getMaxY(), rep1->getMaxZ());
	//
	//vtkSmartPointer<vtkPoints> ugpoints1 = vtkSmartPointer<vtkPoints>::New();
	//vtkSmartPointer<vtkUnstructuredGrid> ug1 = vtkSmartPointer<vtkUnstructuredGrid>::New();
	//vtkSmartPointer<vtkCellArray> cells1 = vtkSmartPointer<vtkCellArray>::New();
	////m_pRenderer->RemoveActor(actorVoxel);//清除已有的actor
	//glDrawCurve2(rep1->m_buff_CurvedLayer, rep1,ugpoints1, ug1, cells1);
	////glDrawCurve(rep->m_buff_CurvedLayer, ugpoints1, ug1, cells1);
	//vtkSmartPointer<vtkDataSetMapper> ugmapper1 = vtkSmartPointer<vtkDataSetMapper>::New();
	//ugmapper1->SetInputData(ug1);
	//vtkSmartPointer<vtkActor> actor1 = vtkSmartPointer<vtkActor>::New();
	//actor1->SetMapper(ugmapper1);
	//actor1->GetProperty()->SetColor(1, 0, 0);
	//m_pRenderer->AddActor(actor1);
	//ui.qvtkWidget->GetRenderWindow()->Render();
	//结束获取圆管曲面**********************




	int countVoxelLayer = m_layerHigh / rep->getRessolution();//每一层高对应的体素层层数

	//对实体进行分层
	int countLayer = rep->getMaxZ() / countVoxelLayer;
	for (int n = 0; n <countLayer; n++)
	{
		rep->makeZCurvedLayer2(n*countVoxelLayer);//从下至上求取该层的曲面刀具
		//rep->makeZCurvedLayer(n*countVoxelLayer);//从上至下求取曲面层
		
		Byte* buff;
		unsigned long long  totalsize = ((unsigned long long)rep->getMaxX() + 1)*
			((unsigned long long)rep->getMaxY() + 1)*((unsigned long long)rep->getMaxZ() + 1);
		buff = new Byte[totalsize];
		memset(buff, 0, totalsize);
		rep->getCurvedLayer(buff);

		buff_curvedLayer.push_back(buff);
	}

	//体素展示
	m_pRenderer->RemoveActor(actorVoxel);//清除已有的actor
	vector<vtkActor * > curvedActor(buff_curvedLayer.size());
	for (int i = 0; i < buff_curvedLayer.size(); i++)
	{
		curvedActor.at(i) = vtkActor::New();
	}
	for (int i = 0; i < buff_curvedLayer.size(); i++)
	{
		vtkSmartPointer<vtkPoints> ugpoints = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
		vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
		glDrawCurve(buff_curvedLayer.at(i), ugpoints, ug, cells);
		vtkSmartPointer<vtkDataSetMapper> ugmapper1 = vtkSmartPointer<vtkDataSetMapper>::New();
		ugmapper1->SetInputData(ug);
		curvedActor.at(i)->SetMapper(ugmapper1);

		if (i%2 == 0)
		{
			curvedActor.at(i)->GetProperty()->SetColor(1,0,0);
		}
		else
		{
			curvedActor.at(i)->GetProperty()->SetColor(0,1,0);
		}
		//curvedActor.at(i)->GetProperty()->SetColor(1, 1, 1/*double(220.0/255.0), double(230.0/255.0), double(240.0/255.0)*/);
		m_pRenderer->AddActor(curvedActor.at(i));
		ui.qvtkWidget->GetRenderWindow()->Render();
	}

	//测试代码,测试显示最终的层数
	int yCoefficient = rep->getMaxX() + 1;//计算缓冲区位置的系数
	int zCoefficient = yCoefficient*(rep->getMaxY() + 1);
	for (int i = 0; i < buff_curvedLayer.size(); i++)
	{
		for (int z = 1; z < rep->getMaxZ(); z++)
		{
			for (int y = 1; y < rep->getMaxY(); y++)
			{
				for (int x = 1; x < rep->getMaxX(); x++)
				{
					if (buff_curvedLayer.at(i)[z*zCoefficient + y*yCoefficient + x] == 1)
					{
						qDebug() << i << " ";
					}
				}
			}
		}
		cout << endl;
	}

}

void test2::glDrawCurve(Byte *buffer, vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells)   //用VTK显示实体体素的
{
	int yCoefficient = rep->getMaxX() + 1;//计算缓冲区位置的系数
	int zCoefficient = yCoefficient*(rep->getMaxY() + 1);
	
	int pointsId = 0;
	for (int z = 1; z < rep->getMaxZ(); z++)
	{
		for (int y = 1; y < rep->getMaxY(); y++)
		{
			for (int x = 1; x < rep->getMaxX(); x++)
			{
				if (buffer[z*zCoefficient + y*yCoefficient + x] ==1)
				{
					float fz = rep->toDoubleZ(z);
					cells->InsertNextCell(8);
					ugpoints->InsertNextPoint((x - 1)*rep->getRessolution(), (y - 1)*rep->getRessolution(), (fz));//换算成这个点的实际坐标值
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x)*rep->getRessolution(), (y - 1)*rep->getRessolution(), (fz));
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x - 1)*rep->getRessolution(), (y)*rep->getRessolution(), (fz));
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x)*rep->getRessolution(), (y)*rep->getRessolution(), (fz));
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x - 1)*rep->getRessolution(), (y - 1)*rep->getRessolution(), (fz)+rep->getRessolution());
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x)*rep->getRessolution(), (y - 1)*rep->getRessolution(), (fz)+rep->getRessolution());
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x - 1)*rep->getRessolution(), (y)*rep->getRessolution(), (fz)+rep->getRessolution());
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x)*rep->getRessolution(), (y)*rep->getRessolution(), (fz)+rep->getRessolution());
					cells->InsertCellPoint(pointsId++);
				}
			}
		}
	}
	ug->SetPoints(ugpoints);
	ug->SetCells(VTK_VOXEL, cells);
}


void test2::glDrawCurve2(Byte *buffer, CZ3DRepresentation *rep1, vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells)   //用VTK显示实体体素的
{
	int yCoefficient = rep1->getMaxX() + 1;//计算缓冲区位置的系数
	int zCoefficient = yCoefficient*(rep1->getMaxY() + 1);

	int pointsId = 0;
	for (int z = 1; z < rep1->getMaxZ(); z++)
	{
		for (int y = 1; y < rep1->getMaxY(); y++)
		{
			for (int x = 1; x < rep1->getMaxX(); x++)
			{
				if (buffer[z*zCoefficient + y*yCoefficient + x] == 1)
				{
					float fz = rep1->toDoubleZ(z);
					cells->InsertNextCell(8);
					ugpoints->InsertNextPoint((x - 1)*rep1->getRessolution(), (y - 1)*rep1->getRessolution(), (fz));//换算成这个点的实际坐标值
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x)*rep1->getRessolution(), (y - 1)*rep1->getRessolution(), (fz));
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x - 1)*rep1->getRessolution(), (y)*rep1->getRessolution(), (fz));
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x)*rep1->getRessolution(), (y)*rep1->getRessolution(), (fz));
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x - 1)*rep1->getRessolution(), (y - 1)*rep1->getRessolution(), (fz)+rep1->getRessolution());
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x)*rep1->getRessolution(), (y - 1)*rep1->getRessolution(), (fz)+rep1->getRessolution());
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x - 1)*rep1->getRessolution(), (y)*rep1->getRessolution(), (fz)+rep1->getRessolution());
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x)*rep1->getRessolution(), (y)*rep1->getRessolution(), (fz)+rep1->getRessolution());
					cells->InsertCellPoint(pointsId++);
				}
			}
		}
	}
	ug->SetPoints(ugpoints);
	ug->SetCells(VTK_VOXEL, cells);
}



void test2::ZigzagButton_clicked()
{
	double internalCurve = zigzagDialog->intervalParameter;
	int intervalY = internalCurve / rep->getRessolution();//路径曲线之间的间隔
	CurvePath newCurvePath;
	newCurvePath.clear();
	double intervalX = 2/ rep->getRessolution();//路径点之间的间隔

	int yCoefficient = rep->getMaxX() + 1;//计算缓冲区位置的系数
	int zCoefficient = yCoefficient*(rep->getMaxY() + 1);

	for (int k = 0; k < buff_curvedLayer.size(); k++)
	{
		//曲面平面化
		//第一步构建一个大小为XY的二维动态数组
		int m = rep->getMaxX();
		int n = rep->getMaxY();
		vector<vector<int> >planeData(m, vector<int>(n));
		//将一维的buff数组值传给二维数组,完成曲面平面化
		for (int i = 0; i < m; i++)//数组初始化
		{
			for (int j = 0; j < n; j++)
			{
				planeData[i][j] = 0;
			}
		}
		for (int z = 0; z < rep->getMaxZ(); z++)
		{
			for (int x = 0; x < rep->getMaxX(); x++)
			{
				for (int y = 0; y < rep->getMaxY(); y++)
				{
					if (buff_curvedLayer.at(k)[z*zCoefficient + y*yCoefficient + x] == 1)
					{
						planeData[x][y] = z;
					}
				}
			}
		}

		//确定打印起止点
		//用Y向扫描线遍历。找到起止点，并在起止点的范围内进行路径点输出
		//构建一个动态数组用来存放起止点
		for (int i = 0; i < m; i+=intervalY)
		{
			if (i%(intervalY*2) ==0)//Y正向扫描
			{
				vector<int> startPoint;
				vector<int> endPoint;
				for (int j = 1; j < n; j++)
				{
					if (planeData[i][j] > 0 && planeData[i][j + 1] > 0 && planeData[i][j - 1] == 0)
					{
						startPoint.push_back(j);
					}

					else if (planeData[i][j] > 0 && planeData[i][j + 1] == 0 && planeData[i][j - 1] > 0)
					{
						endPoint.push_back(j);
					}
				}

				//按一定间隔生成路径点
				for (int p = 0; p < startPoint.size(); p++)
				{
					//保存起点
					NormalPoint normalPointStart(rep->getRessolution()*i, rep->getRessolution()*startPoint.at(p), rep->getRessolution()*planeData[i][startPoint.at(p)], 1, 0, 0);//1代表起点
					newCurvePath << normalPointStart;

					int ll = 0;
					//会出现一种特殊情况，相邻两点的间隔大于目标间隔intervalX，
					for (int q = startPoint.at(p); q < endPoint.at(p); q += ll)
					{
						//寻找下一个合适的间隔点
						if (sqrt(1 + (planeData[i][q + 1] - planeData[i][q])*(planeData[i][q + 1] - planeData[i][q])) > intervalX)
						{
							NormalPoint normalPoint(rep->getRessolution()*i, rep->getRessolution()*(q + 1), rep->getRessolution()*planeData[i][q + 1], 0, 0, 0);
							newCurvePath << normalPoint;
							ll = 1;
						}
						else
						{
							for (int l = 2; l <= intervalX; l++)
							{
								if (q + l + 1 <= rep->getMaxY() - 1)////防止vector数组planeData越界
								{
									double distanceAB = sqrt(l*l + (planeData[i][q + l] - planeData[i][q])*(planeData[i][q + l] - planeData[i][q]));
									double distanceAC = sqrt((l + 1)*(l + 1) + (planeData[i][q + l + 1] - planeData[i][q])*(planeData[i][q + l + 1] - planeData[i][q]));
									if (distanceAB <= intervalX && distanceAC >= intervalX)
									{
										NormalPoint normalPoint(rep->getRessolution()*i, rep->getRessolution()*(q + l), rep->getRessolution()*planeData[i][q + l], 0, 0, 0);
										newCurvePath << normalPoint;
										ll = l;
										break;
									}
								}
								else
									ll = 1;
							}
						}
						//结束寻找下一个合适的间隔点
					}

					//保存终点
					NormalPoint normalPointEnd(rep->getRessolution()*i, rep->getRessolution()*endPoint.at(p), rep->getRessolution()*planeData[i][endPoint.at(p)], 2, 0, 0);//2代表终点
					newCurvePath << normalPointEnd;
				}
				startPoint.clear();
				endPoint.clear();
			}

			else//Y反向扫描
			{
				vector<int> startPoint;
				vector<int> endPoint;
				for (int j = n - 1; j >= 1; j--)
				{
					if (planeData[i][j] > 0 && planeData[i][j - 1] > 0 && planeData[i][j + 1] == 0)
					{
						startPoint.push_back(j);
					}

					else if (planeData[i][j] > 0 && planeData[i][j - 1] == 0 && planeData[i][j + 1] > 0)
					{
						endPoint.push_back(j);
					}
				}

				//按一定间隔生成路径点
				for (int p = 0; p < startPoint.size(); p++)
				{
					//保存起点
					NormalPoint normalPointStart(rep->getRessolution()*i, rep->getRessolution()*startPoint.at(p), rep->getRessolution()*planeData[i][startPoint.at(p)], 1, 0, 0);
					newCurvePath << normalPointStart;

					int ll = 0;
					for (int q = startPoint.at(p); q > endPoint.at(p); q -= ll)
					{
						//寻找下一个合适的间隔点
						if (sqrt(1 + (planeData[i][q - 1] - planeData[i][q])*(planeData[i][q - 1] - planeData[i][q])) > intervalX)
						{
							NormalPoint normalPoint(rep->getRessolution()*i, rep->getRessolution()*(q - 1), rep->getRessolution()*planeData[i][q - 1], 0, 0, 0);
							newCurvePath << normalPoint;
							ll = 1;
						}
						else
						{
							for (int l = 2; l <= intervalX; l++)
							{
								if (q - l - 1 >= 0)//防止vector数组planeData越界
								{
									double distanceAB = sqrt(l*l + (planeData[i][q - l] - planeData[i][q])*(planeData[i][q - l] - planeData[i][q]));
									double distanceAC = sqrt((l + 1)*(l + 1) + (planeData[i][q - l - 1] - planeData[i][q])*(planeData[i][q - l - 1] - planeData[i][q]));
									if (distanceAB <= intervalX && distanceAC >= intervalX)
									{
										NormalPoint normalPoint(rep->getRessolution()*i, rep->getRessolution()*(q - l), rep->getRessolution()*planeData[i][q - l], 0, 0, 0);
										newCurvePath << normalPoint;
										ll = l;
										break;
									}
								}
								else
									ll = 1;
							}
						}
						//结束寻找下一个合适的间隔点
					}

					//保存终点
					NormalPoint normalPointEnd(rep->getRessolution()*i, rep->getRessolution()*endPoint.at(p), rep->getRessolution()*planeData[i][endPoint.at(p)], 2, 0, 0);
					newCurvePath << normalPointEnd;
				}
				startPoint.clear();
				endPoint.clear();
			}
			
			if (newCurvePath.size()>0)
			{
				m_surfacePath << newCurvePath;
				newCurvePath.clear();
			}
		}
		if (m_surfacePath.size()>0)
		{
			m_solidPath << m_surfacePath;
			m_surfacePath.clear();
		}
	}



	//显示路径
	//第一步：清除已有的所有actor
	m_pRenderer->RemoveAllViewProps();
	//第二步：显示路径体素
	vector<vtkActor *> actorPath;
	for (size_t j = 0; j < m_solidPath.at(0).size(); j++)
	{
		for (size_t k = 0; k < m_solidPath.at(0).at(j).size(); k++)
		{
			vtkActor * newActor = vtkActor::New();
			actorPath.push_back(newActor);
			vtkSmartPointer<vtkPoints> ugpoints = vtkSmartPointer<vtkPoints>::New();
			vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
			vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
			int x = m_solidPath.at(0).at(j).at(k).P().X;
			int y = m_solidPath.at(0).at(j).at(k).P().Y;
			int z = m_solidPath.at(0).at(j).at(k).P().P().Z;
			glDrawPath(ugpoints, ug, cells, x, y, z);
			vtkSmartPointer<vtkDataSetMapper> ugmapper1 = vtkSmartPointer<vtkDataSetMapper>::New();
			ugmapper1->SetInputData(ug);
			actorPath.back()->SetMapper(ugmapper1);
			actorPath.back()->GetProperty()->SetColor(0, 0, 0/*double(220.0/255.0), double(230.0/255.0), double(240.0/255.0)*/);
			m_pRenderer->AddActor(actorPath.back());
			ui.qvtkWidget->GetRenderWindow()->Render();
		}
	}
	
	

	//测试代码
	for (size_t i = 0; i < m_solidPath.size(); i++)
	{
		for (size_t j = 0; j < m_solidPath.at(i).size(); j++)
		{
			for (size_t k = 0; k < m_solidPath.at(i).at(j).size(); k++)
			{
				qDebug()<< "x " << m_solidPath.at(i).at(j).at(k).P().X
					<< ", y " << m_solidPath.at(i).at(j).at(k).P().Y
					<< ", z " << m_solidPath.at(i).at(j).at(k).P().P().Z
					<< endl;
			}
		}
	}

}

void test2::OutlineButton_clicked()
{
	double internalCurve = (outlineDialog->intervalParameter)/rep->getRessolution();
	
	for (int k = 0; k < 1/*buff_curvedLayer.size()*/; k++)
	{
		//曲面平面化
		//第一步构建一个大小为XY的二维动态数组
		int m = rep->getMaxX();
		int n = rep->getMaxY();
		vector<vector<int> >planeData(m, vector<int>(n));
		int yCoefficient = rep->getMaxX() + 1;//计算缓冲区位置的系数
		int zCoefficient = yCoefficient*(rep->getMaxY() + 1);
		//将一维的buff数组值传给二维数组,完成曲面平面化
		for (int i = 0; i < m; i++)//数组初始化
		{
			for (int j = 0; j < n; j++)
			{
				planeData[i][j] = 0;
			}
		}
		for (int z = 0; z < rep->getMaxZ(); z++)
		{
			for (int x = 0; x < rep->getMaxX(); x++)
			{
				for (int y = 0; y < rep->getMaxY(); y++)
				{
					if (buff_curvedLayer.at(k)[z*zCoefficient + y*yCoefficient + x] == 1)
					{
						planeData[x][y] = z;
					}
				}
			}
		}//结束曲面“平面化”



		m_pRenderer->RemoveAllViewProps();
		vector<vtkActor *> actorPath2;

		//直接显示该体素，之后把外轮廓往里面偏置一个距离，再来显示一下
		for (int i = 0; i < rep->getMaxX(); i++)
		{
			for (int j = 0; j < rep->getMaxY(); j++)
			{
				int number = 0;
				int number2 = 0;
				if (planeData[i][j] > 0 && planeData[i][j + 1] > 0 && planeData[i][j - 1] == 0)
				{
					if (number%2 == 0)
					{
						//外轮廓
						vtkActor * newActor = vtkActor::New();
						actorPath2.push_back(newActor);
						vtkSmartPointer<vtkPoints> ugpoints = vtkSmartPointer<vtkPoints>::New();
						vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
						vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
						int x = i;
						int y = j;
						int z = planeData[i][j];
						glDrawPath(ugpoints, ug, cells, x, y, z);
						vtkSmartPointer<vtkDataSetMapper> ugmapper1 = vtkSmartPointer<vtkDataSetMapper>::New();
						ugmapper1->SetInputData(ug);
						actorPath2.back()->SetMapper(ugmapper1);
						actorPath2.back()->GetProperty()->SetColor(1, 0, 0/*double(220.0/255.0), double(230.0/255.0), double(240.0/255.0)*/);
						m_pRenderer->AddActor(actorPath2.back());
						ui.qvtkWidget->GetRenderWindow()->Render();


						number++;
					}
					else
					{
						//内轮廓
						vtkActor * newActor = vtkActor::New();
						actorPath2.push_back(newActor);
						vtkSmartPointer<vtkPoints> ugpoints = vtkSmartPointer<vtkPoints>::New();
						vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
						vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
						int x = i;
						int y = j;
						int z = planeData[i][j];
						glDrawPath(ugpoints, ug, cells, x, y, z);
						vtkSmartPointer<vtkDataSetMapper> ugmapper1 = vtkSmartPointer<vtkDataSetMapper>::New();
						ugmapper1->SetInputData(ug);
						actorPath2.back()->SetMapper(ugmapper1);
						actorPath2.back()->GetProperty()->SetColor(0, 1, 0/*double(220.0/255.0), double(230.0/255.0), double(240.0/255.0)*/);
						m_pRenderer->AddActor(actorPath2.back());
						ui.qvtkWidget->GetRenderWindow()->Render();
						number++;
					}
					
				}

				else if (planeData[i][j] > 0 && planeData[i][j + 1] == 0 && planeData[i][j - 1] > 0)
				{
					if (number2%2 == 0)
					{
						//内轮廓
						vtkActor * newActor = vtkActor::New();
						actorPath2.push_back(newActor);
						vtkSmartPointer<vtkPoints> ugpoints = vtkSmartPointer<vtkPoints>::New();
						vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
						vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
						int x = i;
						int y = j;
						int z = planeData[i][j];
						glDrawPath(ugpoints, ug, cells, x, y, z);
						vtkSmartPointer<vtkDataSetMapper> ugmapper1 = vtkSmartPointer<vtkDataSetMapper>::New();
						ugmapper1->SetInputData(ug);
						actorPath2.back()->SetMapper(ugmapper1);
						actorPath2.back()->GetProperty()->SetColor(1, 0, 0/*double(220.0/255.0), double(230.0/255.0), double(240.0/255.0)*/);
						m_pRenderer->AddActor(actorPath2.back());
						ui.qvtkWidget->GetRenderWindow()->Render();

						number2++;
					}
					else
					{
						//外轮廓
						vtkActor * newActor = vtkActor::New();
						actorPath2.push_back(newActor);
						vtkSmartPointer<vtkPoints> ugpoints = vtkSmartPointer<vtkPoints>::New();
						vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
						vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
						int x = i;
						int y = j;
						int z = planeData[i][j];
						glDrawPath(ugpoints, ug, cells, x, y, z);
						vtkSmartPointer<vtkDataSetMapper> ugmapper1 = vtkSmartPointer<vtkDataSetMapper>::New();
						ugmapper1->SetInputData(ug);
						actorPath2.back()->SetMapper(ugmapper1);
						actorPath2.back()->GetProperty()->SetColor(0, 1, 0/*double(220.0/255.0), double(230.0/255.0), double(240.0/255.0)*/);
						m_pRenderer->AddActor(actorPath2.back());
						ui.qvtkWidget->GetRenderWindow()->Render();

						number2++;
					}

				}
			}
		}


		//沿X向扫描
		for (int j = 0; j < rep->getMaxY(); j++)
		{
			for (int i = 0; i < rep->getMaxX(); i++)
			{
				int number = 0;
				int number2 = 0;
				if (planeData[i][j] > 0 && planeData[i+1][j] > 0 && planeData[i-1][j] == 0)
				{
					if (number % 2 == 0)
					{
						//外轮廓
						vtkActor * newActor = vtkActor::New();
						actorPath2.push_back(newActor);
						vtkSmartPointer<vtkPoints> ugpoints = vtkSmartPointer<vtkPoints>::New();
						vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
						vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
						int x = i;
						int y = j;
						int z = planeData[i][j];
						glDrawPath(ugpoints, ug, cells, x, y, z);
						vtkSmartPointer<vtkDataSetMapper> ugmapper1 = vtkSmartPointer<vtkDataSetMapper>::New();
						ugmapper1->SetInputData(ug);
						actorPath2.back()->SetMapper(ugmapper1);
						actorPath2.back()->GetProperty()->SetColor(1, 0, 0/*double(220.0/255.0), double(230.0/255.0), double(240.0/255.0)*/);
						m_pRenderer->AddActor(actorPath2.back());
						ui.qvtkWidget->GetRenderWindow()->Render();


						number++;
					}
					else
					{
						//内轮廓
						vtkActor * newActor = vtkActor::New();
						actorPath2.push_back(newActor);
						vtkSmartPointer<vtkPoints> ugpoints = vtkSmartPointer<vtkPoints>::New();
						vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
						vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
						int x = i;
						int y = j;
						int z = planeData[i][j];
						glDrawPath(ugpoints, ug, cells, x, y, z);
						vtkSmartPointer<vtkDataSetMapper> ugmapper1 = vtkSmartPointer<vtkDataSetMapper>::New();
						ugmapper1->SetInputData(ug);
						actorPath2.back()->SetMapper(ugmapper1);
						actorPath2.back()->GetProperty()->SetColor(0, 1, 0/*double(220.0/255.0), double(230.0/255.0), double(240.0/255.0)*/);
						m_pRenderer->AddActor(actorPath2.back());
						ui.qvtkWidget->GetRenderWindow()->Render();
						number++;
					}

				}

				else if (planeData[i][j] > 0 && planeData[i+1][j] == 0 && planeData[i-1][j] > 0)
				{
					if (number2 % 2 == 0)
					{
						//内轮廓
						vtkActor * newActor = vtkActor::New();
						actorPath2.push_back(newActor);
						vtkSmartPointer<vtkPoints> ugpoints = vtkSmartPointer<vtkPoints>::New();
						vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
						vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
						int x = i;
						int y = j;
						int z = planeData[i][j];
						glDrawPath(ugpoints, ug, cells, x, y, z);
						vtkSmartPointer<vtkDataSetMapper> ugmapper1 = vtkSmartPointer<vtkDataSetMapper>::New();
						ugmapper1->SetInputData(ug);
						actorPath2.back()->SetMapper(ugmapper1);
						actorPath2.back()->GetProperty()->SetColor(1, 0, 0/*double(220.0/255.0), double(230.0/255.0), double(240.0/255.0)*/);
						m_pRenderer->AddActor(actorPath2.back());
						ui.qvtkWidget->GetRenderWindow()->Render();

						number2++;
					}
					else
					{
						//外轮廓
						vtkActor * newActor = vtkActor::New();
						actorPath2.push_back(newActor);
						vtkSmartPointer<vtkPoints> ugpoints = vtkSmartPointer<vtkPoints>::New();
						vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
						vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
						int x = i;
						int y = j;
						int z = planeData[i][j];
						glDrawPath(ugpoints, ug, cells, x, y, z);
						vtkSmartPointer<vtkDataSetMapper> ugmapper1 = vtkSmartPointer<vtkDataSetMapper>::New();
						ugmapper1->SetInputData(ug);
						actorPath2.back()->SetMapper(ugmapper1);
						actorPath2.back()->GetProperty()->SetColor(0, 1, 0/*double(220.0/255.0), double(230.0/255.0), double(240.0/255.0)*/);
						m_pRenderer->AddActor(actorPath2.back());
						ui.qvtkWidget->GetRenderWindow()->Render();

						number2++;
					}

				}
			}
		}




	}
	
}

void test2::glDrawPath(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells, int x, int y, int z)
{
	int pointsId = 0;
	cells->InsertNextCell(8);
	ugpoints->InsertNextPoint(x - rep->getRessolution(), y - rep->getRessolution(), z);//换算成这个点的实际坐标值
	cells->InsertCellPoint(pointsId++);

	ugpoints->InsertNextPoint(x, y - rep->getRessolution(), z);
	cells->InsertCellPoint(pointsId++);

	ugpoints->InsertNextPoint(x - rep->getRessolution(), y, z);
	cells->InsertCellPoint(pointsId++);

	ugpoints->InsertNextPoint(x, y, z);
	cells->InsertCellPoint(pointsId++);

	ugpoints->InsertNextPoint(x - rep->getRessolution(), y - rep->getRessolution(), z+rep->getRessolution());
	cells->InsertCellPoint(pointsId++);

	ugpoints->InsertNextPoint(x, y - rep->getRessolution(), z+rep->getRessolution());
	cells->InsertCellPoint(pointsId++);

	ugpoints->InsertNextPoint(x - rep->getRessolution(), y, z+rep->getRessolution());
	cells->InsertCellPoint(pointsId++);

	ugpoints->InsertNextPoint(x, y, z+rep->getRessolution());
	cells->InsertCellPoint(pointsId++);

	ug->SetPoints(ugpoints);
	ug->SetCells(VTK_VOXEL, cells);
}