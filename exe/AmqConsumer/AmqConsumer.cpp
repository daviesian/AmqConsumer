// AmqConsumer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "activemq/library/ActiveMQCPP.h"
#include <cms/Connection.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Config.h>

using namespace std;

cms::Connection* amqConnection;
cms::Session* amqSession;
cms::Destination* amqDestination;
cms::MessageProducer* amqProducer;
cms::MessageConsumer* amqConsumer;

int _tmain(int argc, _TCHAR* argv[])
{
		activemq::library::ActiveMQCPP::initializeLibrary();

		// Create a ConnectionFactory
        auto_ptr<cms::ConnectionFactory> connectionFactory(cms::ConnectionFactory::createCMSConnectionFactory("failover:(tcp://192.168.1.189:61616)"));

        // Create a Connection
        amqConnection = connectionFactory->createConnection();
        amqConnection->start();

        amqSession = amqConnection->createSession(cms::Session::AUTO_ACKNOWLEDGE);


        amqDestination = amqSession->createTopic("ucam_test");



		amqConsumer = amqSession->createConsumer(amqDestination);
		amqConsumer->start();
		while(1)
		{
			cms::Message* m = amqConsumer->receive();

			const cms::MapMessage* mmsg = dynamic_cast<const cms::MapMessage*> (m);

			vector<string, allocator<string>> ns = mmsg->getMapNames();
			cout << endl << endl << "*************  MAP MESSAGE *************" << endl;

			for(int i = 0; i < ns.size(); i++)
			{
				cout << "  " << ns.at(i) << endl;
				cout << "    " << mmsg->getString(ns.at(i)) << endl;
			}
			delete m;
		}

		activemq::library::ActiveMQCPP::shutdownLibrary();
}

