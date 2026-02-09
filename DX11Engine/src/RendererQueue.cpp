#include "RendererQueue.hpp"

std::vector<const Model*> RendererQueue::s_queue;
RendererQueue::ComPtr<ID3D11DeviceContext> RendererQueue::s_deviceContext;

void RendererQueue::AddContext(const ComPtr<ID3D11DeviceContext>& deviceContext)
{
	assert(deviceContext);

	s_deviceContext = deviceContext;
}

void RendererQueue::Enqueue(const Model* model)
{
	s_queue.push_back(model);
}

void RendererQueue::Flush()
{
	assert(s_deviceContext);

	for (const auto& model : s_queue)
	{
		model->Draw(s_deviceContext.Get());
	}

	s_queue.clear();
}
