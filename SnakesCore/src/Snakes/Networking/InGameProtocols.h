#pragma once

#include "Socket.h"
#include "Snakes/Core/UUID.h"

#define RQ_CELLSIZE 0
#define RP_CELLSIZE 1

#define RQ_MAP 2
#define RP_MAP 3

namespace sk::BMTP {

	// BMTP: BitMap Transfer Protocol

	struct Header {
		UUID game_uuid = -1;
		int type;
		int x, y, w, h;
		int cell_size;

		Header() : game_uuid(0), type(-1), cell_size(-1), x(-1), y(-1), w(-1), h(-1) {}
	};

	namespace Client {

		size_t CellSizeRequests(
			Ref<Socket> client,
			const std::string& sv_ip, int sv_port, 
			UUID game_uuid
		);
		
		// bufffer: have to be of size cell_size * w * h
		bool MapRequest(
			Ref<Socket> client,
			const std::string& sv_ip, int sv_port, 
			UUID game_uuid, 
			int x, int y, int w, int h,
			size_t cell_size,
			void* buffer
		);

	}
	
	namespace Server {

		// @return: Request Code
		int GetRequest(
			Ref<Socket> server,
			std::string& cl_ip, int& cl_port, 
			UUID& game_uuid, 
			int& x, int& y, int& w, int& h
		);
		
		void SendCellSize(
			Ref<Socket> server,
			const std::string& cl_ip, int cl_port, 
			UUID game_uuid, 
			size_t cell_size
		);
		
		bool SendMap(
			Ref<Socket> server,
			const std::string& cl_ip, int cl_port, 
			UUID game_uuid, 
			int x, int y, int w, int h,
			size_t cell_size,
			const void* map
		);
	
	}

}

namespace sk::CVP {

	// CVP: Change Velocity Protocol
	
	struct Header {
		UUID game_uuid = -1;
		UUID snake_uuid = -1;
		glm::vec2 velocity;
	};
	
	namespace Client {
		
		void VelocityChangeRequest(
			Ref<Socket> client,
			const std::string& sv_ip, int sv_port,
			UUID game_uuid,
			UUID snake_uuid,
			const glm::vec2& velocity
		);

	}

}