/**
 * @file		Channel.h
 * @author  	Sebastian Ritterbusch <ospac@ritterbusch.de>
 * @version 	1.0
 * @date		5.2.2016
 * @copyright	MIT License (see LICENSE file)
 * @brief 		Audio channel abstraction
 */

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <vector>

/**
 * @brief Audio channel abstraction class
 *
 * TODO: Switch to auto_ptr<float> to avoid copy operations at assignments.
 * TODO: Virtualization of memory segments to avoid full memory operations.
 */
class Channel
{
	unsigned     	   rate;
	std::vector<float> data;
	static float	   zero;
public:
	/**
	 * Create a new audio channel.
	 */
	Channel();

	/**
	 * Create an audio channel with given sample rate
	 * @param rate sample rate in Hertz (1/s)
	 */
	Channel(unsigned rate);

	/**
	 * Create an audio channel with given rate and sample data
	 * @param rate sample rate in Hertz (1/s)
	 * @param data audio data as float vector
	 */
	Channel(unsigned rate, const std::vector<float> & data);

	/**
	 * Create an audio channel with given rate and number of samples
	 * @param rate sample rate in Hetz (1/s)
	 * @param size number of samples
	 */
	Channel(unsigned rate, unsigned size);

	virtual ~Channel();

	/**
	 * Access a sample for read/write access
	 * The bounds are checked on the index and an impostor is returned
	 * in case of out-of-bounds requests.
	 * @param index of sample
	 * @return float reference on sample
	 */
	float & operator [](int);

	/**
	 * Access to a sample with read only access
	 * The bounds are checked on the index and zero is returned in case of
	 * out-of-bounds requests.
	 * @param index of sample
	 * @return float value of sample
	 */
	float   operator [](int) const;

	/**
	 * Number of samples in this channel
	 * @return number of samples
	 */
	unsigned size() const;

	/**
	 * Sample rate of this channel
	 * @return sample rate in Hertz (1/s)
	 */
	unsigned samplerate() const;

	/**
	 * Returns the l2-norm of the channel normalized to one sample.
	 * @f[ ||u||_2:=\sqrt{ \frac1n \sum_{i=0}^{n-1} u_i^2 } @f]
	 * @return normalized l2-norm of the channel
	 */
	double  l2norm() const;


	/**
	 * Returns the "up" l2-norm of the channel normalized to one sample,
	 * taking only the values into account that are higher than the given limit L.
	 * This hints to the energy of the channels when it is active, when
	 * invoked with the l2norm of the channel as limit.
	 * @f[ ||u||^{>L}_2:=\sqrt{ \frac1m \sum_{j=0}^{m-1} v_j^2 }, \textrm{ where } \forall i: u_i> L \exists_1 j(i): v_{j(i)}=u_i @f]
	 * @param  limit value
	 * @return normalized "up" l2-norm of the channel
	 */
	double  l2upnorm(float) const;

	/**
	 * Returns the "down" l2-norm of the channel normalized to one sample,
	 * taking only the values into account that are higher than the given limit L.
	 * This hints to the energy of the channels when it is not active, when
	 * invoked with the l2norm of the channel as limit.
	 * @f[ ||u||^{<L}_2:=\sqrt{ \frac1m \sum_{j=0}^{m-1} v_j^2 }, \textrm{ where } \forall i: u_i< L \exists_1 j(i): v_{j(i)}=u_i @f]
	 * @param  limit value
	 * @return normalized "up" l2-norm of the channel
	 */
	double  l2downnorm(float) const;

	/**
	 * Maximum absolute sample value
	 * @return maximum absolute sample value
	 */
	double  linfnorm() const;

	/**
	 * Downsample the channel by given factor.
	 * @param factor downsample factor
	 * @return new channel with a new sample frequency divided by the factor
	 */
	Channel downsample(unsigned) const;

	/**
	 * Downsample the channel by given factor and square the values
	 * @param factor downsample factor
	 * @return new channel with sample frequency divided by the factor
	 */
	Channel downsampleEnergy(unsigned) const;


	/**
	 * Create a copy of this channel with given size
	 * @param size new number of samples
	 * @return channel with given number of samples
	 */
	Channel resizeTo(unsigned) const;

	/**
	 * Create a copy of this channel with given sample rate
	 * @param newRate sample rate of target channel
	 * @return channel with given sample rate
	 */
	Channel resampleTo(unsigned) const;

private:
	template<class T>
	static T sqr(const T&a){return a*a;}

};
/**
 * Vector of channels as type for multiple channels.
 */
typedef std::vector<Channel> Channels;

#endif /* CHANNEL_H_ */
