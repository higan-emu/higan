auto Stream::create(uint channels, double frequency) -> void {
  setChannels(channels);
  setFrequency(frequency, audio.frequency);
  audio.append(*this);
}

auto Stream::destroy() -> void {
  audio.remove(*this);
}

auto Stream::setChannels(uint channelCount) -> void {
  channels.reset();
  channels.resize(channelCount);
}

auto Stream::setFrequency(double inputFrequency, maybe<double> outputFrequency) -> void {
  this->inputFrequency = inputFrequency;
  if(outputFrequency) this->outputFrequency = outputFrequency();

  for(auto& channel : channels) {
    channel.nyquist.reset();
    channel.resampler.reset(this->inputFrequency, this->outputFrequency);
  }

  if(this->inputFrequency >= this->outputFrequency * 2) {
    //add a low-pass filter to prevent aliasing during resampling
    double cutoffFrequency = min(25000.0, this->outputFrequency / 2.0 - 2000.0);
    for(auto& channel : channels) {
      uint passes = 3;
      for(uint pass : range(passes)) {
        DSP::IIR::Biquad filter;
        double q = DSP::IIR::Biquad::butterworth(passes * 2, pass);
        filter.reset(DSP::IIR::Biquad::Type::LowPass, cutoffFrequency, this->inputFrequency, q);
        channel.nyquist.append(filter);
      }
    }
  }
}

auto Stream::resetFilters() -> void {
  for(auto& channel : channels) {
    channel.filters.reset();
  }
}

auto Stream::addLowPassFilter(double cutoffFrequency, Filter::Order order, uint passes) -> void {
  for(auto& channel : channels) {
    for(uint pass : range(passes)) {
      if(order == Filter::Order::First) {
        Filter filter{Filter::Mode::OnePole, Filter::Type::LowPass, Filter::Order::First};
        filter.onePole.reset(DSP::IIR::OnePole::Type::LowPass, cutoffFrequency, inputFrequency);
        channel.filters.append(filter);
      }
      if(order == Filter::Order::Second) {
        Filter filter{Filter::Mode::Biquad, Filter::Type::LowPass, Filter::Order::Second};
        double q = DSP::IIR::Biquad::butterworth(passes * 2, pass);
        filter.biquad.reset(DSP::IIR::Biquad::Type::LowPass, cutoffFrequency, inputFrequency, q);
        channel.filters.append(filter);
      }
    }
  }
}

auto Stream::addHighPassFilter(double cutoffFrequency, Filter::Order order, uint passes) -> void {
  for(auto& channel : channels) {
    for(uint pass : range(passes)) {
      if(order == Filter::Order::First) {
        Filter filter{Filter::Mode::OnePole, Filter::Type::HighPass, Filter::Order::First};
        filter.onePole.reset(DSP::IIR::OnePole::Type::HighPass, cutoffFrequency, inputFrequency);
        channel.filters.append(filter);
      }
      if(order == Filter::Order::Second) {
        Filter filter{Filter::Mode::Biquad, Filter::Type::HighPass, Filter::Order::Second};
        double q = DSP::IIR::Biquad::butterworth(passes * 2, pass);
        filter.biquad.reset(DSP::IIR::Biquad::Type::HighPass, cutoffFrequency, inputFrequency, q);
        channel.filters.append(filter);
      }
    }
  }
}

auto Stream::addLowShelfFilter(double cutoffFrequency, Filter::Order order, double gain, double slope) -> void {
  for(auto& channel : channels) {
    if(order == Filter::Order::Second) {
      Filter filter{Filter::Mode::Biquad, Filter::Type::LowShelf, Filter::Order::Second};
      double q = DSP::IIR::Biquad::shelf(gain, slope);
      filter.biquad.reset(DSP::IIR::Biquad::Type::LowShelf, cutoffFrequency, inputFrequency, q);
      channel.filters.append(filter);
    }
  }
}

auto Stream::addHighShelfFilter(double cutoffFrequency, Filter::Order order, double gain, double slope) -> void {
  for(auto& channel : channels) {
    if(order == Filter::Order::Second) {
      Filter filter{Filter::Mode::Biquad, Filter::Type::HighShelf, Filter::Order::Second};
      double q = DSP::IIR::Biquad::shelf(gain, slope);
      filter.biquad.reset(DSP::IIR::Biquad::Type::HighShelf, cutoffFrequency, inputFrequency, q);
      channel.filters.append(filter);
    }
  }
}

auto Stream::pending() const -> bool {
  return channels && channels[0].resampler.pending();
}

auto Stream::read(double samples[]) -> uint {
  for(uint c : range(channels.size())) samples[c] = channels[c].resampler.read();
  return channels.size();
}

auto Stream::write(const double samples[]) -> void {
  for(auto c : range(channels.size())) {
    double sample = samples[c] + 1e-25;  //constant offset used to suppress denormals
    for(auto& filter : channels[c].filters) {
      switch(filter.mode) {
      case Filter::Mode::OnePole: sample = filter.onePole.process(sample); break;
      case Filter::Mode::Biquad: sample = filter.biquad.process(sample); break;
      }
    }
    for(auto& filter : channels[c].nyquist) {
      sample = filter.process(sample);
    }
    channels[c].resampler.write(sample);
  }

  audio.process();
}
