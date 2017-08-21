#include <iostream>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/Point>
#include <osg/PointSprite>
#include <osg/Texture2D>
#include <osg/BlendFunc>
#include <osg/Material>
#include <osg/StateSet>
#include <osgGA/FirstPersonManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/UFOManipulator>
#include <osgGA/GUIEventHandler>
#include <osgGA/CameraManipulator>
#include <osgGA/StateSetManipulator>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ModularProgram>
#include <osgParticle/AccelOperator>
#include <osgText/Text>
#include <osgText/Font>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgViewer/ViewerEventHandlers>


using namespace osg;

bool gogglesOn = false;
bool once = false;
bool text1 = false;
bool text2 = false;
bool text3 = false;


// ModelController will inherit GUIeventhandler class for keyboard input events
class ModelController : public osgGA::GUIEventHandler
{
	public:	
	ModelController( osg::MatrixTransform* node , osg::MatrixTransform* goggle)
	: _model(node), _goggle (goggle)
	{}
	virtual bool handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa );
	protected:
	osg::ref_ptr<osg::MatrixTransform> _model;
	osg::ref_ptr<osg::MatrixTransform> _goggle;
};

// plays different actions on keyboard events
bool ModelController::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	if ( !_model ) return false;
	if ( !_goggle ) return false;
	osg::Matrix matrix = _model->getMatrix();
	osg::Matrix goggleMatrix = _goggle->getMatrix();
	
	switch ( ea.getEventType() )
	{
		case osgGA::GUIEventAdapter::KEYDOWN:
		switch ( ea.getKey() )
		{
			case 'o': case 'O':
			matrix *= osg::Matrix::rotate(-0.5f, osg::Z_AXIS);
			break;
			case 'c': case 'C':
			matrix *= osg::Matrix::rotate(0.5f, osg::Z_AXIS);
			break;			
			case 'p': case 'P':
			goggleMatrix *= osg::Matrix::scale(0.0f,0.0f,0.0f);
			gogglesOn = true;
			break;

			default:
			break;
		}
	_model->setMatrix( matrix );
	_goggle->setMatrix( goggleMatrix );
	break;
	default:
	break;
	}
return false;
}

//particle spray
osgParticle::ParticleSystem* createParticleSystem(osg::Group* parent )
{
	osg::ref_ptr<osgParticle::ParticleSystem> ps = new osgParticle::ParticleSystem;
	ps->getDefaultParticleTemplate().setShape(osgParticle::Particle::POINT );
	
	osg::ref_ptr<osg::BlendFunc> blendFunc = new osg::BlendFunc;
	blendFunc->setFunction( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	texture->setImage( osgDB::readImageFile("smoke.rgb"));
	
	osg::StateSet* ss = ps->getOrCreateStateSet();
	ss->setAttributeAndModes( blendFunc.get() );
	ss->setTextureAttributeAndModes( 0, texture.get() );
	ss->setAttribute( new osg::Point(20.0f) );
	ss->setTextureAttributeAndModes( 0, new osg::PointSprite );
	ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF);
	ss->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
	
	osg::ref_ptr<osgParticle::RandomRateCounter> rrc = new osgParticle::RandomRateCounter;
	rrc->setRateRange( 500, 800 );

	osg::ref_ptr<osgParticle::ModularEmitter> emitter = new osgParticle::ModularEmitter;
	emitter->setParticleSystem( ps.get() );
	emitter->setCounter( rrc.get() );
	
	osg::ref_ptr<osgParticle::AccelOperator> accel = new osgParticle::AccelOperator;
	accel->setToGravity();

	osg::ref_ptr<osgParticle::ModularProgram> program = new osgParticle::ModularProgram;
	program->setParticleSystem( ps.get() );
	program->addOperator( accel.get() );

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable( ps.get() );
	parent->addChild( emitter.get() );
	parent->addChild( program.get() );
	parent->addChild( geode.get() );
	return ps.get();
}

int main(int argc, char** argv)
{
	//will create the lab model using cube shapes
	osg::ref_ptr<osg::ShapeDrawable> leftWall = new osg::ShapeDrawable;
	leftWall->setShape(new osg::Box(osg::Vec3(-10.0f, 0.0f, 0.0f), 0.1f, 20.0f, 10.0f));
	leftWall->setColor(osg::Vec4(0.4f, 0.4f, 0.4f, 0.0f));   //red, green, blue, black

	osg::ref_ptr<osg::ShapeDrawable> rightWall = new osg::ShapeDrawable;
	rightWall->setShape(new osg::Box(osg::Vec3(10.0f, 0.0f, 0.0f), 0.1f, 20.0f, 10.0f));
	rightWall->setColor(osg::Vec4(0.4f, 0.4f, 0.4f, 0.0f));

	osg::ref_ptr<osg::ShapeDrawable> backWall = new osg::ShapeDrawable;
	backWall->setShape(new osg::Box(osg::Vec3(0.0f, 10.0f, 0.0f), 20.0f, 0.1f, 10.0f));
	backWall->setColor(osg::Vec4(0.4f, 0.4f, 0.4f, 0.0f));

	osg::ref_ptr<osg::ShapeDrawable> frontWallLeft = new osg::ShapeDrawable;
	frontWallLeft->setShape(new osg::Box(osg::Vec3(-6.0f, -10.0f, 0.0f), 8.0f, 0.1f, 10.0f));
	frontWallLeft->setColor(osg::Vec4(0.4f, 0.4f, 0.4f, 0.0f));

	osg::ref_ptr<osg::ShapeDrawable> frontWallRight = new osg::ShapeDrawable;
	frontWallRight->setShape(new osg::Box(osg::Vec3(6.0f, -10.0f, 0.0f), 8.0f, 0.1f, 10.0f));
	frontWallRight->setColor(osg::Vec4(0.4f, 0.4f, 0.4f, 0.0f));

	osg::ref_ptr<osg::ShapeDrawable> door = new osg::ShapeDrawable;
	door->setShape(new osg::Box(osg::Vec3(0.0f, -10.0f, 0.0f), 4.0f, 0.1f, 10.0f));
	door->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 0.0f));

	osg::ref_ptr<osg::ShapeDrawable> floor = new osg::ShapeDrawable;
	floor->setShape(new osg::Box(osg::Vec3(0.0f, 0.0f, -5.0f), 20.0f, 20.0f, 0.1f));

	osg::ref_ptr<osg::ShapeDrawable> tableLeft = new osg::ShapeDrawable;
	tableLeft->setShape(new osg::Box(osg::Vec3(-8.5f, 0.0f, -3.0f), 3.0f, 16.0f, 4.0f)); //lenth, width, height
	tableLeft->setColor(osg::Vec4(0.9f, 0.8f, 0.5f, 0.0f));

	osg::ref_ptr<osg::ShapeDrawable> tableRight = new osg::ShapeDrawable;
	tableRight->setShape(new osg::Box(osg::Vec3(8.5f, 0.0f, -3.0f), 3.0f, 16.0f, 4.0f)); //lenth, width, height
	tableRight->setColor(osg::Vec4(0.9f, 0.8f, 0.5f, 0.0f));

	osg::ref_ptr<osg::ShapeDrawable> tableBack = new osg::ShapeDrawable;
	tableBack->setShape(new osg::Box(osg::Vec3(0.0f, 8.5f, -3.0f), 10.0f, 3.0f, 4.0f));
	tableBack->setColor(osg::Vec4(0.0f, 0.0f, 0.7f, 0.0f));
	
	osg::ref_ptr<osg::ShapeDrawable> tableOut = new osg::ShapeDrawable;
	tableOut->setShape(new osg::Box(osg::Vec3(7.5f, -15.0f, -3.0f), 3.0f, 3.0f, 4.0f)); //lenth, width, height

	//left tubes
	osg::ref_ptr<osg::ShapeDrawable> tube1 = new osg::ShapeDrawable;
	tube1->setShape(new osg::Cylinder(osg::Vec3(-8.5f, -1.0f, -0.5f), 0.1f, 1.0f));
	osg::ref_ptr<osg::ShapeDrawable> tube2 = new osg::ShapeDrawable;
	tube2->setShape(new osg::Cylinder(osg::Vec3(-8.5f, -0.5f, -0.5f), 0.1f, 1.0f));
	osg::ref_ptr<osg::ShapeDrawable> tube3 = new osg::ShapeDrawable;
	tube3->setShape(new osg::Cylinder(osg::Vec3(-8.5f, 0.0f, -0.5f), 0.1f, 1.0f));
	osg::ref_ptr<osg::ShapeDrawable> tube4 = new osg::ShapeDrawable;
	tube4->setShape(new osg::Cylinder(osg::Vec3(-8.5f, 0.5f, -0.5f), 0.1f, 1.0f));

	//right tubes
	osg::ref_ptr<osg::ShapeDrawable> tube5 = new osg::ShapeDrawable;
	tube5->setShape(new osg::Cylinder(osg::Vec3(8.5f, -3.0f, -0.5f), 0.1f, 1.0f));
	osg::ref_ptr<osg::ShapeDrawable> tube6 = new osg::ShapeDrawable;
	tube6->setShape(new osg::Cylinder(osg::Vec3(8.5f, -2.5f, -0.5f), 0.1f, 1.0f));
	osg::ref_ptr<osg::ShapeDrawable> tube7 = new osg::ShapeDrawable;
	tube7->setShape(new osg::Cylinder(osg::Vec3(8.5f, -2.0f, -0.5f), 0.1f, 1.0f));
	osg::ref_ptr<osg::ShapeDrawable> tube8 = new osg::ShapeDrawable;
	tube8->setShape(new osg::Cylinder(osg::Vec3(8.5f, -1.5f, -0.5f), 0.1f, 1.0f));
	
	//loading 3D model files and applying manipulations to it
	osg::ref_ptr<osg::Node> fire = osgDB::readNodeFile("SmokeBox.osgt");	
	osg::ref_ptr<osg::Node> stool = osgDB::readNodeFile("chl009.3DS");
	osg::ref_ptr<osg::Node> cylinder = osgDB::readNodeFile("gasCylinder.3ds");
	
	osg::ref_ptr<osg::MatrixTransform> cylinderTransformLeft = new osg::MatrixTransform;
	osg::Matrix cylinderPosLeft = osg::Matrix::translate(-2000.0f, 1900.0f, -1300.0f);
	osg::Matrix cylinderScaleLeft = osg::Matrix::scale(0.004f,0.004f,0.004f);
	cylinderTransformLeft->setMatrix(cylinderPosLeft * cylinderScaleLeft);
	cylinderTransformLeft->addChild(cylinder);

	osg::ref_ptr<osg::MatrixTransform> cylinderTransformRight = new osg::MatrixTransform;
	osg::Matrix cylinderPosRight = osg::Matrix::translate(1300.0f, 1900.0f, -1300.0f);
	osg::Matrix cylinderScaleRight = osg::Matrix::scale(0.004f,0.004f,0.004f);
	cylinderTransformRight->setMatrix(cylinderPosRight * cylinderScaleRight);
	cylinderTransformRight->addChild(cylinder);
	
	
	osg::ref_ptr<osg::Node> chemReactor = osgDB::readNodeFile("chemDevice.3ds");
	osg::ref_ptr<osg::MatrixTransform> chemTransform = new osg::MatrixTransform;
	osg::Matrix chemPos = osg::Matrix::translate(-10.0f, 70.0f, -10.0f);
	osg::Matrix chemScale = osg::Matrix::scale(0.09f,0.09f,0.09f);
	chemTransform->setMatrix(chemPos * chemScale);
	chemTransform->addChild(chemReactor);

	osg::ref_ptr<osg::MatrixTransform> stoolTransform = new osg::MatrixTransform;	
	osg::Matrix posStool = osg::Matrix::translate(90.0f, 30.0f, -85.0f);            // right side back
	osg::Matrix sizeStool = osg::Matrix::scale(0.06f, 0.06f, 0.06f);
	osg::Matrix resultMatStool = posStool * sizeStool;
	stoolTransform->setMatrix( resultMatStool );	
	stoolTransform->addChild(stool.get());
	
	osg::ref_ptr<osg::MatrixTransform> stoolTransformFront = new osg::MatrixTransform;	
	osg::Matrix posStoolFront = osg::Matrix::translate(90.0f, -60.0f, -85.0f);            // right side front
	osg::Matrix sizeStoolFront = osg::Matrix::scale(0.06f, 0.06f, 0.06f);
	osg::Matrix resultMatStoolFront = posStoolFront * sizeStoolFront;
	stoolTransformFront->setMatrix( resultMatStoolFront);	
	stoolTransformFront->addChild( stool.get() );
		
	osg::ref_ptr<osg::MatrixTransform> stoolTransformLeft = new osg::MatrixTransform;	
	osg::Matrix posStoolLeft = osg::Matrix::translate(-90.0f, 0.0f, -85.0f);            // leftSide
	osg::Matrix sizeStoolLeft = osg::Matrix::scale(0.06f, 0.06f, 0.06f);
	osg::Matrix resultMatStoolLeft = posStoolLeft * sizeStoolLeft;
	stoolTransformLeft->setMatrix( resultMatStoolLeft);	
	stoolTransformLeft->addChild( stool.get() );
	
	osg::ref_ptr<osg::Node> glassNode = osgDB::readNodeFile("gasMask.obj");
	osg::ref_ptr<osg::MatrixTransform> transform1 = new osg::MatrixTransform;	
	osg::Matrix pos = osg::Matrix::translate(1500.0f, 750.0f, -250.0f);
	osg::Matrix rotate = osg::Matrix::rotate(1.5f, Vec3(0.0f, 0.0f, -600.0f));
	osg::Matrix size = osg::Matrix::scale(0.01f, 0.01f, 0.01f);
	osg::Matrix resultMat = pos * size * rotate;
	transform1->setMatrix( resultMat );	
	transform1->addChild( glassNode.get() );

	osg::ref_ptr<osg::Node> tubeHolder = osgDB::readNodeFile("tubeholder.dae");	
	osg::ref_ptr<osg::MatrixTransform> tubeHolderTrans = new osg::MatrixTransform;	
	osg::Matrix tubeHolderpos = osg::Matrix::translate(0.0f, 0.0f, 0.0f);
	osg::Matrix tubeHoldersize = osg::Matrix::scale(1000.0f, 1000.0f, 1000.0f);
	osg::Matrix tubeHolderresultMat = tubeHolderpos * tubeHoldersize;
	tubeHolderTrans->setMatrix( tubeHolderresultMat );	
	tubeHolderTrans->addChild( tubeHolder.get());


	//adding texts to file
	osg::ref_ptr<osgText::Font> g_font = osgText::readFontFile("arial.ttf");
	
	osg::ref_ptr< osgText::Text > textLabInfo = new osgText::Text;
	textLabInfo->setText( "Welcome To Lab. You have choice to follow safety here.\npress 'p' to wear mask\npress 'o' to open Lab door." );
	textLabInfo->setFont( g_font.get() );
	//textLabInfo->setPosition( osg::Vec3( -15.0, -20.0, 0.0 ) );
	textLabInfo->setCharacterSize( 0.5f );
	textLabInfo->setAxisAlignment( osgText::TextBase::XZ_PLANE );	
	osg::ref_ptr<osg::Geode> textGeode = new osg::Geode;
	textGeode->addDrawable(textLabInfo.get());

	osg::ref_ptr< osgText::Text > textInLab = new osgText::Text;
	textInLab->setText( "Please walk toward the reactor." );
	textInLab->setFont( g_font.get() );
	//textInLab->setPosition( osg::Vec3( 0.0, 0.0, 0.0 ) );
	textInLab->setCharacterSize( 0.5f );
	textInLab->setAxisAlignment( osgText::TextBase::XZ_PLANE );	
	osg::ref_ptr<osg::Geode> textInLabGeode = new osg::Geode;
	textInLabGeode->addDrawable(textInLab.get());

	osg::ref_ptr< osgText::Text > textBlind = new osgText::Text;
	textBlind->setText( "Because of your poor decision\nYou have gone blind\nYou no longer can see anything" );
	textBlind->setFont( g_font.get() );
	//textBlind->setPosition( osg::Vec3( 15.0, -20.0, 0.0 ) );
	textBlind->setCharacterSize( 0.5f );
	textBlind->setAxisAlignment( osgText::TextBase::XZ_PLANE );	
	osg::ref_ptr<osg::Geode> textBlindGeode = new osg::Geode;
	textBlindGeode->addDrawable(textBlind.get());

	//Door Animation : Start
	osg::ref_ptr<osg::Geode> doorGeode = new osg::Geode;
	doorGeode->addDrawable(door.get());

	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	mt->addChild( doorGeode.get() );
	osg::ref_ptr<ModelController> ctrler = new ModelController( mt.get(), transform1.get() );
	//Door Animation : End

	//Spray : Start
	osg::ref_ptr<osg::MatrixTransform> sprayMt = new osg::MatrixTransform;
	sprayMt->setMatrix( osg::Matrix::translate(0.0f, 8.0f, -2.0f) );
	
	osgParticle::ParticleSystem* ps = createParticleSystem( sprayMt.get());
	osg::ref_ptr<osgParticle::ParticleSystemUpdater> updater = new osgParticle::ParticleSystemUpdater;
	updater->addParticleSystem( ps );
	//Spray : End
	
	//creating tubeGeode for 8 tubes in the model then adding it to root node
	osg::ref_ptr<osg::Geode> tubeGeode = new osg::Geode;
	tubeGeode->addDrawable(tube1.get());
	tubeGeode->addDrawable(tube2.get());
	tubeGeode->addDrawable(tube3.get());
	tubeGeode->addDrawable(tube4.get());
	tubeGeode->addDrawable(tube5.get());
	tubeGeode->addDrawable(tube6.get());
	tubeGeode->addDrawable(tube7.get());
	tubeGeode->addDrawable(tube8.get());


	osg::ref_ptr<osg::Geode> root1 = new osg::Geode;
	root1->addDrawable(leftWall.get());
	root1->addDrawable(rightWall.get());
	root1->addDrawable(backWall.get());
	root1->addDrawable(frontWallLeft.get());
	root1->addDrawable(frontWallRight.get());
	root1->addDrawable(floor.get());
	root1->addDrawable(tableLeft.get());
	root1->addDrawable(tableRight.get());
	root1->addDrawable(tableBack.get());
	root1->addDrawable(tableOut.get());

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(root1.get());
	root->addChild(tubeGeode.get());
	root->addChild( mt.get() );
	root->addChild(transform1.get() );
	root->addChild(stoolTransform.get());
	root->addChild(stoolTransformFront.get());
	root->addChild(stoolTransformLeft.get());
	root->addChild(chemTransform.get());
	root->addChild(cylinderTransformLeft.get());
	root->addChild(cylinderTransformRight.get());
	root->addChild(tubeHolderTrans.get());
	
	//starting a viewer and adding scene to it plus the event handler for keyboard input plus trackball Manipulator
	osgViewer::Viewer viewer;
	// press o to open the door, c to close the door and p to pick up face mask
	viewer.addEventHandler( ctrler.get() );
	viewer.getCamera()->setAllowEventFocus( true );
	viewer.setSceneData(root);
	// left click+movement for angle, right click + movement to go forward backward, middle click + movement to go sideways
	viewer.setCameraManipulator(new osgGA::TrackballManipulator);
	
	bool textdisappear1 = false;
	osg::ref_ptr<osg::MatrixTransform> textTrans = new osg::MatrixTransform;
	osg::ref_ptr<osg::MatrixTransform> textTrans1 = new osg::MatrixTransform;
	
while ( !viewer.done() )
	{
		osg::Vec3 eye,center,up; 
		viewer.getCamera()-> getViewMatrixAsLookAt(eye,center,up);
		//std::cout << center[0] <<"  "<< center[1] <<"  "<< center[2] << std::endl;
		viewer.frame();
	
		if (center[0] <= 4.0 && center[0] >= -4.0 && center[1] <= -30.0 && center[1] >= -80.0 && text1 == false){
			osg::Matrix textpos = osg::Matrix::translate(center[0] - 15.0, center[1] + 50.0, center[2]);            
			textTrans->setMatrix(textpos);	
			textTrans->addChild(textGeode.get());
			root->addChild(textTrans.get());
			text1 = true;	
		}
		if (center[0] <= 4.0 && center[0] >= -4.0 && center[1] <= -10.0 && center[1] >= -40.0 && text1 == true){
			root->removeChild(textTrans.get());
		}
		
		if (center[0] <= 4.0 && center[0] >= -4.0 && center[1] <= -1.0 && center[1] >= -10.0 && text2 == false){
			osg::Matrix textpos1 = osg::Matrix::translate(center[0] - 5.0, center[1] + 12.0, center[2] + 2.0);            
			textTrans1->setMatrix(textpos1);	
			textTrans1->addChild(textInLabGeode.get());
			root->addChild(textTrans1.get());
			std::cout << "this is in lab." << std::endl;
			text2 = true;	
		}
		
		if (center[0] <= 4.0 && center[0] >= -4.0 && center[1] <= 10.0 && center[1] >= 0.0 && text2 == true){
			root->removeChild(textTrans1.get());
		}
		
		// User has touched the reactor, so activate the spray, smoke 
		if (center[1] <= 4.0 && center[1] >= 2.0 && center[2] <= 4.0 && center[2] >= -3.0 && once == false){
			root->addChild( updater.get() );
			root->addChild( sprayMt.get() );
			root->addChild(fire.get());
			once = true;
		}
		
		if (once == true && gogglesOn == false){
			osg::ref_ptr<osg::MatrixTransform> smokeBurn = new osg::MatrixTransform;
			osg::Matrix smokePos = osg::Matrix::translate(center[0], center[1], center[2]);            
			smokeBurn->setMatrix(smokePos);	
			smokeBurn->addChild(fire.get());
			root->addChild(smokeBurn.get());
			osg::ref_ptr<osg::MatrixTransform> textTrans2 = new osg::MatrixTransform;
			osg::Matrix textpos2 = osg::Matrix::translate(center[0] - 5.0, center[1] + 3.0, center[2] + 2.0);            
			textTrans2->setMatrix(textpos2);	
			textTrans2->addChild(textBlindGeode.get());
			if (text3 == false){
				root->addChild(textTrans2.get());
				text3 = true;
			}				
			
		}
	}
	return 0;
}
