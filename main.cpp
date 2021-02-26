#define __STDCPP_WANT_MATH_SPEC_FUNCS__ 1

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <limits>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <boost/math/special_functions/beta.hpp>
#include "readdata.h"
#include "classes.h"


#define STOP 1.0e-8
#define TINY 1.0e-30

//stolen incomplete beta function cuz im too lazy to implement
double incbeta(double a, double b, double x) {
    if (x < 0.0 || x > 1.0) return 1.0/0.0;

    /*The continued fraction converges nicely for x < (a+1)/(a+b+2)*/
    if (x > (a+1.0)/(a+b+2.0)) {
        return (1.0-incbeta(b,a,1.0-x)); /*Use the fact that beta is symmetrical.*/
    }

    /*Find the first part before the continued fraction.*/
    const double lbeta_ab = lgamma(a)+lgamma(b)-lgamma(a+b);
    const double front = exp(log(x)*a+log(1.0-x)*b-lbeta_ab) / a;

    /*Use Lentz's algorithm to evaluate the continued fraction.*/
    double f = 1.0, c = 1.0, d = 0.0;

    int i, m;
    for (i = 0; i <= 200; ++i) {
        m = i/2;

        double numerator;
        if (i == 0) {
            numerator = 1.0; /*First numerator is 1.0.*/
        } else if (i % 2 == 0) {
            numerator = (m*(b-m)*x)/((a+2.0*m-1.0)*(a+2.0*m)); /*Even term.*/
        } else {
            numerator = -((a+m)*(a+b+m)*x)/((a+2.0*m)*(a+2.0*m+1)); /*Odd term.*/
        }

        /*Do an iteration of Lentz's algorithm.*/
        d = 1.0 + numerator * d;
        if (fabs(d) < TINY) d = TINY;
        d = 1.0 / d;

        c = 1.0 + numerator / c;
        if (fabs(c) < TINY) c = TINY;

        const double cd = c*d;
        f *= cd;

        /*Check for stop.*/
        if (fabs(1.0-cd) < STOP) {
            return front * (f-1.0);
        }
    }

    return 1.0/0.0; /*Needed more loops, did not converge.*/
}

double commulDistribution(double v, double t)
{
	double incbetaVal = (v/(pow(t,2)+v));
	double incbetaAns = incbeta((v/2),0.5,incbetaVal);
	/*
	std::cout << "func value small thingy yeah: "  << (v/(pow(t,2)+v)) << std::endl;
	std::cout << "inc beta func: " << incbetaAns << std::endl;
	std::cout << "real beta func: "  << std::beta((v/2),0.5) << std::endl;
	double regularizedIncBeta = incbetaAns/std::beta((v/2),0.5);*/
	
	double commulDistr = 1-incbetaAns/2;
	
	return commulDistr;
}

double invCommulDistribution(double v, double t)
{
	double signVal = (t-0.5) < 0 ? -1: 1;
	double pVal = t<0.5 ? (2*t) : (2*(1-t));
	double val2Divis = boost::math::ibeta_inv((v/2),0.5,pVal);
	double val2 = v*(1/val2Divis-1);
	double val2Sqrt = sqrt(val2);
	/*std::cout << "(signVal: "  << signVal << ")" << std::endl;
	std::cout << "(pVal: "  << pVal << ")" << std::endl;
	std::cout << "(val2Divis: "  << val2Divis << ")" << std::endl;
	std::cout << "(val2: "  << val2 << ")" << std::endl;*/
	double invCommulDistr = signVal * val2Sqrt;
	
	return invCommulDistr;
}

void windowFunc(int width, int height, std::vector<DataValues> data)
{
	sf::RenderWindow window(sf::VideoMode(width,height),"Graph view");

	Date firstDate = data[0].date;
	Date lastDate = data[data.size()-1].date;
	int dateSpanDays = lastDate.ToDays()-firstDate.ToDays();
	float lowestVar = std::numeric_limits<float>::infinity();
	float highestVar = -std::numeric_limits<float>::infinity();
	for(DataValues datai : data)
	{
		if(lowestVar>datai.value)
		{
			lowestVar=datai.value;
		}
		if(highestVar<datai.value)
		{
			highestVar=datai.value;
		}
	}

	float highlowDiff = highestVar-lowestVar;

	float lineWidth = 2.5;
	float textScale = 0.5;
	float statsPointRadius = 4;
	int floatPrecision = 2;


	//drawing bla bla bla
	sf::Font font;
	sf::FloatRect bounds;
	font.loadFromFile("c:/windows/fonts/arial.ttf");
	std::vector<sf::RectangleShape> shapes;
	std::vector<sf::Text> texts;

	sf::RectangleShape lineHor(sf::Vector2f(width-(width/8), lineWidth));
	lineHor.setOrigin((width-(width/8))/2,lineWidth/2);
	lineHor.setPosition(width/1.85,height-(height/6));
	
	shapes.push_back(lineHor);
	
	sf::RectangleShape lineVer(sf::Vector2f(height-(height/5.5), lineWidth));
	lineVer.setOrigin((height-(height/5.5))/2,lineWidth/2);
	lineVer.setPosition((width/1.85)-(width/2)+(width/8)/2,(height-(height/6))-((height-(height/5.5))/2)+lineWidth/2);
	lineVer.setRotation(90);
	
	shapes.push_back(lineVer);
	
	std::ostringstream outstr;
	outstr.precision(floatPrecision);
	outstr << std::fixed << (lowestVar-highlowDiff/2);
	sf::Text lowNumText(outstr.str(),font);
	bounds = lowNumText.getLocalBounds();
	lowNumText.setOrigin(bounds.width,bounds.height);
	lowNumText.setScale(textScale, textScale);
	lowNumText.setPosition((width/1.85)-(width/2)+(width/8)/2-textScale*30,height-(height/6)-textScale*10);
	outstr.str("");
	
	texts.push_back(lowNumText);
	
	outstr << std::fixed << (highestVar+highlowDiff/2);
	sf::Text highNumText(outstr.str(),font);
	bounds = highNumText.getLocalBounds();
	highNumText.setOrigin(bounds.width,0);
	highNumText.setScale(textScale, textScale);
	highNumText.setPosition((width/1.85)-(width/2)+(width/8)/2-bounds.height/2,height-(height-(height/5.5)+(height/6))+textScale*10);
	
	texts.push_back(highNumText);
	
	std::string dateStr = std::to_string(firstDate.year)+"/"+std::to_string(firstDate.month)+"/"+std::to_string(firstDate.day);
	sf::Text lowDateText(dateStr,font);
	bounds = lowDateText.getLocalBounds();
	lowDateText.setOrigin(0,0);
	lowDateText.setScale(textScale, textScale);
	lowDateText.setPosition((width/1.85)-(width/2)+(width/8)/2,height-(height/6)+textScale*10);
	
	texts.push_back(lowDateText);
	
	dateStr = std::to_string(lastDate.year)+"/"+std::to_string(lastDate.month)+"/"+std::to_string(lastDate.day);
	sf::Text highDateText(dateStr,font);
	bounds = highDateText.getLocalBounds();
	highDateText.setOrigin(bounds.width,0);
	highDateText.setScale(textScale, textScale);
	highDateText.setPosition(width/1.85+(width-(width/8))/2,height-(height/6)+textScale*10);
	
	texts.push_back(highDateText);
	
	for(int i = 0; i < texts.size(); i++)
	{
		texts[i].setFillColor(sf::Color(100, 150, 255));
	}
	//drawing over
	
	//drawing the graph now
	
	std::vector<sf::CircleShape> circles;
	
	float topStart = height-(height-(height/5.5)+(height/6));
	float verticalHeight = height-(height/5.5);
	float pixelPerOne = verticalHeight/((highestVar+highlowDiff/2)-(lowestVar-highlowDiff/2));
	float leftStart = (width/1.85)-(width/2)+(width/8)/2;
	float horizontalHeight = width-(width/8);
	float pixelPerOneDate = horizontalHeight/dateSpanDays;
	
	float lastX = 0;
	float lastY = 0;
	
	for(DataValues datapoint : data)
	{
		sf::CircleShape circle;
		circle.setRadius(statsPointRadius);
		circle.setOrigin(statsPointRadius,statsPointRadius);
		float x = leftStart+(dateSpanDays-(lastDate.ToDays()-datapoint.date.ToDays()))*pixelPerOneDate;
		float y = topStart+((highestVar+highlowDiff/2)-datapoint.value)*pixelPerOne;
		circle.setPosition(x,y);
		circles.push_back(circle);
		if(lastX!=0 || lastY!=0)
		{
			float triangleWidth = x-lastX;
			float triangleHeight = y-lastY;
			float triangleBigSide = std::sqrt(std::pow(triangleWidth,2)+std::pow(triangleHeight,2));
			float angle = std::atan2(triangleWidth,-triangleHeight)-M_PI/2;
			
			sf::RectangleShape graphLine(sf::Vector2f(triangleBigSide, lineWidth));
			graphLine.setOrigin(lineWidth/2,lineWidth/2);
			graphLine.setPosition(lastX,lastY);
			graphLine.setRotation(angle*(180/M_PI));
			
			shapes.push_back(graphLine);
		}
		lastX = x;
		lastY = y;
	}
	
	for(int i = 0; i < circles.size(); i++)
	{
		circles[i].setFillColor(sf::Color(100, 150, 255));
	}
	
	for(int i = 0; i < shapes.size(); i++)
	{
		shapes[i].setFillColor(sf::Color(50, 100, 250));
	}
	
	//drawing the graph ends here

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
		for(sf::CircleShape circlei : circles)
		{
			window.draw(circlei);
		}
        for(sf::RectangleShape rectanglei : shapes)
		{
			window.draw(rectanglei);
		}
		for(sf::Text texti : texts)
		{
			window.draw(texti);
		}
        window.display();
    }
}

void calculateStats(std::string datapath, std::string searchstr)
{
	std::vector<DataValues> data;
	
	readfile(datapath, data);
	
	std::cout << std::endl << std::endl;
	
	std::vector<DataValues> varVec;
	double averageVariable = 0;
	
	for(int i = 0; i < data.size(); i++)
	{
		std::cout << data[i].name << " (" << data[i].date.day << "): " << data[i].value << std::endl;
		if(data[i].name.find(searchstr)!=std::string::npos)
		{
			averageVariable+=data[i].value;
			varVec.push_back(data[i]);
		}
	}
	
	averageVariable/=varVec.size();
	
	double sStandardDev = 0;
	for(int i = 0; i < varVec.size(); i++)
	{
		sStandardDev+=pow(averageVariable-varVec[i].value,2);
	}
	sStandardDev = sqrt(sStandardDev/(varVec.size()-1));
	
	double sdStandardDev = sStandardDev/(sqrt(varVec.size()));
	
	double tvalue = invCommulDistribution(varVec.size()-1,0.975);
	double marginOfError = tvalue*sdStandardDev;
	
	std::cout << std::endl;
	std::cout << "Average variable: " << averageVariable << std::endl;
	std::cout << "Sample standard deviation: " << sStandardDev << std::endl;
	std::cout << "Sampling distribution standard deviation: " << sdStandardDev << std::endl;
	std::cout << "T Value for 95%, " << varVec.size()-1 << " df: " << tvalue << std::endl;
	std::cout << "Margin of error: " << marginOfError << std::endl;
	
	std::cout << "\nReal value between " << averageVariable-marginOfError << " and " << averageVariable+marginOfError << std::endl;
	
	windowFunc(640,480,varVec);
}

int main()
{
	std::string datapath;
	std::string searchstr;

	std::cout << "path to data: " << std::endl;
	getline(std::cin, datapath);
	
	std::cout << "variable name: " << std::endl;
	getline(std::cin, searchstr);
	while(true)
	{
		calculateStats(datapath, searchstr);
		
		std::string userAction;
		
		std::cout << "\n\nPress enter to quit" << "\nType path to change path" << "\nType var to change the variable" << "\nType update to run with same settings\n";
		getline(std::cin, userAction);
		if(userAction=="path")
		{
			std::cout << "\nnew path to data: " << std::endl;
			getline(std::cin, datapath);
		}
		else if(userAction=="var")
		{
			std::cout << "\nnew variable name: " << std::endl;
			getline(std::cin, searchstr);
		}
		else if(userAction=="update")
		{
		} else
		{
			break;
		}
	}
	return 0;
}