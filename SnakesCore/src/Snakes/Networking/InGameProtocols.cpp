#include "skpch.h"
#include "InGameProtocols.h"

#include "Socket.h"

namespace sk::BMTP {

	namespace Client {
	
		size_t CellSizeRequest(Ref<Socket> client, const std::string& sv_ip, int sv_port, UUID game_uuid) {
			// Send request to the server
			Header req;
			req.game_uuid = game_uuid;
			req.type = RQ_CELLSIZE;
			client->SendTo(&req, sizeof(req), sv_ip, sv_port);

			// Receive the cell size from the server
			Header reply;
			std::string incoming_ip;
			int incoming_port = 0;
			// TEMP:
			// ignoring the infomation except for the cell_size
			client->ReceiveFrom(&reply, sizeof(reply), incoming_ip, incoming_port);
			return reply.cell_size;
		}

		bool MapRequest(Ref<Socket> client, const std::string& sv_ip, int sv_port, UUID game_uuid, int x, int y, int w, int h, size_t cell_size, void* buffer) {
			if (!buffer)
				return false;
			
			// Send request to the server
			Header req;
			req.game_uuid = game_uuid;
			req.type = RQ_MAP;
			req.x = x, req.y = y, req.w = w, req.h = h;
			cell_size = req.cell_size;
			client->SendTo(&req, sizeof(req), sv_ip, sv_port);

			// Receive the map from the server
			int buf_len = sizeof(Header) + cell_size * w * h;
			void* rep_buffer = malloc(buf_len);
			if (!rep_buffer)
				return false;

			std::string incoming_ip;
			int incoming_port = 0;
			// TEMP:
			// ignoring the incoming ip, port and the UUID returned from the server
			int msg_len = client->ReceiveFrom(rep_buffer, sizeof(buf_len), incoming_ip, incoming_port);
			
			Header rep_header = *(Header*)rep_buffer;

			memcpy(buffer, (char*)rep_buffer + sizeof(rep_header), cell_size * w * h);
			free(rep_buffer);
			
			return true;
		}

	}

	namespace Server {

		int GetRequest(Ref<Socket> server, std::string& cli_ip, int& cli_port, UUID& game_uuid, int& x, int& y, int& w, int& h) {
			Header cli_req;
			// 'cli_ip' and 'cli_port' are written here
			int msg_len = server->ReceiveFrom(&cli_req, sizeof(cli_req), cli_ip, cli_port);

			game_uuid = cli_req.game_uuid;
			if (cli_req.type == RQ_CELLSIZE) {
				x = y = w = h = -1;
				return RQ_CELLSIZE;
			}

			if (cli_req.type == RQ_MAP) {
				x = cli_req.x;
				y = cli_req.y;
				w = cli_req.w;
				h = cli_req.h;
				return RQ_MAP;
			}
			return -1;
		}

		void SendCellSize(Ref<Socket> server, const std::string& cl_ip, int cl_port, UUID game_uuid, size_t cell_size) {
			Header rep;
			rep.game_uuid = game_uuid;
			rep.type = RP_CELLSIZE;
			rep.cell_size = cell_size;

			server->SendTo(&rep, sizeof(rep), cl_ip, cl_port);
		}

		bool SendMap(Ref<Socket> server, const std::string& cl_ip, int cl_port, UUID game_uuid, int x, int y, int w, int h, size_t cell_size, const void* map) {
			Header rep;
			rep.game_uuid = game_uuid;
			rep.type = RP_MAP;
			rep.x = x, rep.y = y, rep.w = w, rep.h = h;
			rep.cell_size = cell_size;

			size_t map_size = cell_size * w * h;
			size_t buf_len = sizeof(Header) + map_size;
			void* buffer = malloc(buf_len);
			if (!buffer)
				return false;

			memcpy(buffer, &rep, sizeof(rep));
			memcpy((char*)buffer + sizeof(size_t), map, map_size);
			
			server->SendTo(buffer, buf_len, cl_ip, cl_port);
			free(buffer);
			return true;
		}
	}

}

namespace sk::CVP::Client {

	void VelocityChangeRequest(Ref<Socket> client, const std::string& sv_ip, int sv_port, UUID game_uuid, UUID snake_uuid, const glm::vec2& velocity) {
		Header req;
		req.game_uuid = game_uuid;
		req.snake_uuid = snake_uuid;
		req.velocity = velocity;
		client->SendTo(&req, sizeof(req), sv_ip, sv_port);
	}

}