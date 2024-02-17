#pragma once
#include "skpch.h"

#include "Snakes/Core/Core.h"
#include "Snakes/Renderer/VertexArray.h"

namespace sk {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetVertexBuffer(Ref<VertexBuffer> vertexBuffer) override;
		virtual void SetIndexBuffer(Ref<IndexBuffer> indexBuffer) override;
		
		virtual Ref<VertexBuffer> GetVertexBuffer() const override { return m_VertexBuffer; }
		virtual Ref<IndexBuffer> GetIndexBuffer() const override { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};

}